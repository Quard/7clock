#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "system.h"
#include "display.h"
#include "rtc.h"
#include "ambient.h"


uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);


int main(void) {
    system_init();
    display_init();
    rtc_init();
    ambient_init();

    sei();

    ambient_start_measuring();
    rtc_set_time(18, 34);

    display_brightness_init();
    display_brightness_set(100);

    uint8_t minutes = 43, hours = 9;
    uint16_t ambient_light = 4321;
    uint32_t i = 0;
    rtc_get_datetime(&hours, &minutes);
    while (1) {
        i++;

	display_show_clock(hours, minutes);
	/* display_sign(ambient_light % 10, 0xFF & ~(1 << 3)); */
	/* display_sign(ambient_light / 10 % 10, 0xFF & ~(1 << 2)); */
	/* display_sign(ambient_light / 100 % 10, 0xFF & ~(1 << 1)); */
	/* display_sign(ambient_light / 1000 % 10, 0xFF & ~(1 << 0)); */
        
        if (i > 1500) { //10000) {
	  i = 0;
	  rtc_get_datetime(&hours, &minutes);
	  ambient_light = ambient_get();
	  display_brightness_set(DISPLAY_MIN_BRIGHTNESS - (uint8_t)map(ambient_light, 0, 1024, 0, DISPLAY_MIN_BRIGHTNESS));

	  ambient_start_measuring();
        }
    }   
    
    return 0;
}

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

