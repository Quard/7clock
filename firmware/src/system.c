#include "system.h"
#include <avr/interrupt.h>
#include "peripherals/twi.h"
#include "device.h"


void _system_systick_init(void);


volatile static uint32_t tick;


void system_init(void) {
  // Clock config
  // 20Mhz / 10 = 2Mhz
  CCP = 0xD8;
  CLKCTRL.MCLKCTRLB = (0x9 << 1) | 1;
  CCP = 0x00;

  _system_systick_init();
  twi_init();
}

uint32_t get_tick(void) {
  return tick;
}

void system_btn_init(void) {
  PORTA.DIR &= ~(1 << BUTTON_PIN);
  PORTA.PIN6CTRL = 0x01; // Both edges
}

void _system_systick_init(void) {
  TCA0.SINGLE.CTRLB = 0;
  TCA0.SINGLE.CTRLD = 0;
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
  TCA0.SINGLE.PER = 2000;
  TCA0.SINGLE.CTRLECLR = TCA_SINGLE_DIR_bm;
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}

ISR(TCA0_OVF_vect) {
  TCA0.SINGLE.INTFLAGS = 1;

  tick++;
}
