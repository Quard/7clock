#include "display.h"


const static uint8_t SIGNS[] = {
  //EDCdBAFG
  0b00010001,  // 0
  0b11010111,  // 1
  0b00110010,  // 2
  0b10010010,  // 3
  0b11010100,  // 4
  0b10011000,  // 5
  0b00011000,  // 6
  0b11010011,  // 7
  0b00010000,  // 8
  0b10010000,  // 9
};

void _display_send_data(uint8_t value);


void display_init(void) {
  PORTA.DIR |= (1 << DISPLAY_DS_PIN) | (1 << DISPLAY_STCP_PIN) | (1 << DISPLAY_SHCP_PIN) | (1 << DISPLAY_BRIGHNESS_PIN);
}


void display(uint8_t value, uint8_t idx) {
  PORTA.OUT &= ~(1 << DISPLAY_STCP_PIN);
  _display_send_data(value);
  _display_send_data(idx);
  PORTA.OUT |= (1 << DISPLAY_STCP_PIN);
}

void display_sign(uint8_t sign, uint8_t idx) {
  display(SIGNS[sign], idx);
}

void display_show_clock(uint8_t hours, uint8_t minutes, uint8_t display_mask) {
  if (display_mask & (1 << 2)) display_sign(minutes / 10, 0xFF & ~(1 << 2));
  if (display_mask & (1 << 3)) display_sign(minutes % 10, 0xFF & ~(1 << 3));
  if (display_mask & (1 << 0)) display_sign(hours / 10, 0xFF & ~(1 << 0));
  if (display_mask & (1 << 1)) display_sign(hours % 10, 0xFF & ~(1 << 1));
}

void display_brightness_init(void) {
  TCB0_CTRLB = (0x07 << 0) | (1 << 4);
  TCB0_CCMPL = DISPLAY_MIN_BRIGHTNESS;
  TCB0_CCMPH = 5;
  TCB0_CTRLA = TCB_ENABLE_bm;
}

void display_brightness_set(uint8_t val) {
  TCB0_CCMPH = val;
}

void _display_send_data(uint8_t value) {
  for (uint8_t i=0; i<8; i++) {
    if ((value & 128) == 0) PORTA.OUT &= ~(1 << DISPLAY_DS_PIN);
    else PORTA.OUT |= (1 << DISPLAY_DS_PIN);

    value <<= 1;

    PORTA.OUT |= (1 << DISPLAY_SHCP_PIN);
    PORTA.OUT &= ~(1 << DISPLAY_SHCP_PIN);
  }
}
