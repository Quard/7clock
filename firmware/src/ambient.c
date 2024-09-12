#include "ambient.h"
#include <avr/interrupt.h>
#include <util/delay.h>


volatile static uint16_t ambient_value;


void ambient_init(void) {
  PORTA.DIR &= ~(1 << 4);
  PORTA.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;

  ADC0.CTRLD = (0x04 << ADC_SAMPDLY_gp) | (0x03 << ADC_INITDLY_gp);
  ADC0.CTRLC = ADC_PRESC_DIV2_gc | ADC_REFSEL_VDDREF_gc | ADC_SAMPCAP_bm;
  ADC0.CTRLB = 0x03 << ADC_SAMPNUM_gp;
  ADC0.SAMPCTRL = 3;
  ADC0.INTCTRL = ADC_RESRDY_bm;
  ADC0.CTRLA = ADC_ENABLE_bm; // | ADC_RESSEL_10BIT_gc;
  ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;
}

void ambient_start_measuring(void) {
  ADC0.COMMAND = ADC_STCONV_bm;
}

uint16_t ambient_get(void) {
  return ambient_value;
}

ISR(ADC0_RESRDY_vect) {
  ADC0.INTFLAGS = 0xFF;
  ambient_value = ADC0.RES >> 3;
}
