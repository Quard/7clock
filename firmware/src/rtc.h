#ifndef __RTC_H__
#define	__RTC_H__

#include <avr/io.h>
#include "device.h"


#define RTC_PCF8523_I2C_ADDR 0b1101000


void rtc_init(void);
void rtc_get_datetime(uint8_t *hours, uint8_t *minutes);
void rtc_set_time(uint8_t hours, uint8_t minutes);

#endif	/* __RTC_H__ */

