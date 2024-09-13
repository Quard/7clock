#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "system.h"
#include "display.h"
#include "rtc.h"
#include "ambient.h"


uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);


volatile static uint8_t btn_state;
volatile static uint32_t btn_timestamp;


int main(void) {
  system_init();
  display_init();
  rtc_init();
  ambient_init();
  system_btn_init();

  sei();

  ambient_start_measuring();
  rtc_set_time(18, 34);

  display_brightness_init();
  display_brightness_set(100);

  uint8_t minutes = 43, hours = 9;
  uint16_t ambient_light = 4321;
  uint32_t update = 0;
  rtc_get_datetime(&hours, &minutes);
  while (1) {
    // display_show_clock(hours, minutes);
    /* display_sign(ambient_light % 10, 0xFF & ~(1 << 3)); */
    /* display_sign(ambient_light / 10 % 10, 0xFF & ~(1 << 2)); */
    /* display_sign(ambient_light / 100 % 10, 0xFF & ~(1 << 1)); */
    /* display_sign(ambient_light / 1000 % 10, 0xFF & ~(1 << 0)); */
    display_sign(btn_state % 10, 0xFF & ~(1 << 0));
    if (btn_timestamp == 0) display_sign(0, 0xFF & ~(1 << 3));
    else display_sign(1, 0xFF & ~(1 << 3));

    if (get_tick() - update > 1000) { // 256ms
      ambient_light = ambient_get();
      display_brightness_set(DISPLAY_MIN_BRIGHTNESS - (uint8_t)map(ambient_light, 0, 1024, 0, DISPLAY_MIN_BRIGHTNESS));

      ambient_start_measuring();
      rtc_get_datetime(&hours, &minutes);
      update = get_tick();
    }
  }

  return 0;
}

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

ISR(PORTA_PORT_vect) {
  if (PORTA.INTFLAGS & (1 << BUTTON_PIN)) {
    PORTA.INTFLAGS &= 1 << BUTTON_PIN;
    if (PORTA.IN & (1 << BUTTON_PIN)) btn_timestamp = get_tick();
    else {
      uint32_t duration = get_tick() - btn_timestamp;
      if (BUTTON_IS_SHORT_PRESS(duration)) btn_state = 1;
      else if (BUTTON_IS_LONG_PRESS(duration)) btn_state = 2;
      btn_timestamp = 0;
    }
  }
}
