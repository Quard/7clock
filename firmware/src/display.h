#ifndef __DISPLAY_H__
#define	__DISPLAY_H__

#include <avr/io.h>
#include "device.h"


#define DISPLAY_MIN_BRIGHTNESS  100


void display_init(void);
void display(uint8_t value, uint8_t idx);
void display_sign(uint8_t sign, uint8_t idx);
void display_show_clock(uint8_t hours, uint8_t minutes, uint8_t display_mask);
void display_brightness_init(void);
void display_brightness_set(uint8_t val);

#endif	/* __DISPLAY_H__ */
