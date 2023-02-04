// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


#include "conf.h"

#include <xc.h>

#include "display.h"
#include "ds1302.h"
#include "button.h"
#include "state_machine.h"


void interrupt_init(void);
void timer_init(void);
void rtc_init(void);
void adc_init(void);
void pwm_init(void);
void btn_init(void);
inline void pwm_set_duty(uint8_t duty);
void brightness_control(void);
ButtonState btn_get_press_type(uint32_t delay);


volatile uint32_t sys_tick;
State state = State_Clock;
volatile ButtonState btn_pressed;
volatile uint32_t btn_last_update;
uint8_t min_brightness;
uint8_t display_mask = 0b10001111;

extern volatile uint8_t _display_spi_sent;

void main(void) {
    OSCCONbits.IRCF = 0b1101;  // 4 MHz

    TRISA = 0;
    TRISB = 0;
    min_brightness = MIN_BRIGHTNESS;

    interrupt_init();
    timer_init();
    display_init();
    rtc_init();
    adc_init();
    pwm_init();
    btn_init();

//    ds1302_set_time(20, 41);

    uint8_t hour = 0xFF, minutes = 0;
    uint32_t last_update = 0;
    uint32_t flash = 0;
    while (1) {
        if (sys_tick - last_update >= 1000) {
            ds1302_get_time(&hour, &minutes);
            brightness_control();
            last_update = sys_tick;
        }

        if (sys_tick - flash > 499) {
            if (state >= State_SetupTimeMinutesL) {
                display_mask ^= 1 << (4 - state);
            }
            flash = sys_tick;
        }

        state_transition_table[state][btn_pressed](&hour, &minutes, &state, (ButtonState *)&btn_pressed);
    }

    return;
}

__interrupt() void ISR(void) {
    if (TMR1IF) {
        sys_tick++;

        TMR1H = 0xFF;
        TMR1L = 0x06;
        TMR1IF = 0;
    }
    if (SSP1IF) {
        _display_spi_sent = 1;
        SSP1IF = 0;
    }
    if (INTF) {
        if (INTEDG) {
            INTEDG = 0;
            btn_last_update = sys_tick;
        } else {
            INTEDG = 1;
            btn_pressed = btn_get_press_type(sys_tick - btn_last_update);
        }
        INTF = 0;
    }
}

void interrupt_init(void) {
    GIE = 1;
    PEIE = 1;
}

void timer_init(void) {
    // SYS TICK timer
    sys_tick = 0;
    TMR1H = 0xFF;
    TMR1L = 0x06;
    TMR1IE = 1;
    T1CON = 0b00110001;
}

void rtc_init(void) {
    if (!ds1302_is_enabled()) {
        ds1302_enable();
    }
}

void adc_init(void) {
    ANSA1 = 1;
    TRISA1 = 1;
    ANSELA = 0x0 | (1 << 1);  // RA1(AN1) - analog
    ADCON1 = 0b11000000; // Right justified, FOSC/4, Vref=Vcc
    ADCON0 = 0b00000111; // AN1 activate, ADC on
}

void pwm_init(void) {
    PR2 = 9;  //(_XTAL_FREQ / (100kHz * 4 * 1)) - 1;
    T2CON = 0b00000100;  // TIM2 on, Prescaler 1
    CCP1CON = 0b00001100;
    TRISB3 = 0;
}

void btn_init(void) {
    btn_pressed = ButtonState_Idle;

    TRISB0 = 1;
    INTEDG = 1;
    INTE = 1;
}

inline void pwm_set_duty(uint8_t duty) {
    CCPR1L = duty;
}

void brightness_control(void) {
    if ((ADCON0 & 0b10) == 0) {
        uint16_t val = (uint16_t)(((ADRESH & 0b11) << 8) | ADRESL);
        if (val < 400) {
            pwm_set_duty(1);
        } else {
            pwm_set_duty(min_brightness);
        }
        ADCON0 |= (1 << 1);
    }
}

ButtonState btn_get_press_type(uint32_t delay) {
    if (delay > 25 && delay < 250) {
        return ButtonState_ShortPress;
    } else if (delay > 500 && delay < 2000) {
        return ButtonState_LongPress;
    }

    return 0;
}
