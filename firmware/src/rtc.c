#include "rtc.h"
#include "peripherals/twi.h"


uint8_t bcd_to_byte(uint8_t data);
uint8_t byte_to_bcd(uint8_t data);


void rtc_init(void) {
}

void rtc_get_datetime(uint8_t *hours, uint8_t *minutes) {
  uint8_t buf[7] = {0};

  twi_read(RTC_PCF8523_I2C_ADDR, 0x03, buf, 7);
        
  *minutes = bcd_to_byte(buf[1] & 0x7F);
  *hours = bcd_to_byte(buf[2] & 0x3F);
}

void rtc_set_time(uint8_t hours, uint8_t minutes) {
  uint8_t buf[] = {byte_to_bcd(minutes), byte_to_bcd(hours)};

  twi_write(RTC_PCF8523_I2C_ADDR, 0x04, buf, 2);
}

uint8_t bcd_to_byte(uint8_t data) {
  return ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

uint8_t byte_to_bcd(uint8_t data) {
  return ((data / 10) << 4) | (data % 10);
}