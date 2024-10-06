#ifndef __RTC_H__
#define	__RTC_H__

#include <avr/io.h>
#include "device.h"


#define RTC_PCF8523_I2C_ADDR 0b1101000

#define PCF8523_REG_CTRL3     0x02
#define PCF8523_REG_SECONDS   0x03
#define PCF8523_REG_MINUTES   0x04

#define PCF8523_CTRL3_PM_val  0b100
#define PCF8523_CTRL3_PM_pos  5


void rtc_init(void);
void rtc_get_datetime(uint8_t *hours, uint8_t *minutes);
void rtc_set_time(uint8_t hours, uint8_t minutes);

#endif	/* __RTC_H__ */

