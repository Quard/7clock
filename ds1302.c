#include "ds1302.h"

void _ds1302_transmission_begin(void);
void _ds1302_transmission_end(void);
void _ds1302_write(uint8_t val, uint8_t is_data_req);
uint8_t _ds1302_read(void);
uint8_t _ds1302_bcd_to_byte(uint8_t data);
uint8_t _ds1302_byte_to_bcd(uint8_t data);


uint8_t ds1302_is_enabled(void) {
    uint8_t enabled = 0;

    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_CH, 1);
    enabled = _ds1302_read();
    _ds1302_transmission_end();

    return enabled & 0b10000000;
}

void ds1302_enable(void) {
    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_CH, 1);
    uint8_t val = _ds1302_read();
    _ds1302_transmission_end();

    val &= ~(1 << 7);

    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_CH - 1, 0);
    _ds1302_write(val, 0);
    _ds1302_transmission_end();
}

uint8_t ds1302_get_writeprotect(void) {
    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_WP, 1);
    uint8_t val = _ds1302_read();
    _ds1302_transmission_end();

    return val & 0b10000000;
}

void ds1302_get_time(uint8_t *hour, uint8_t *minutes) {
    uint8_t seconds = 0;
    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_MINUTES, 1);
    seconds = _ds1302_read();
    _ds1302_transmission_end();
    seconds = _ds1302_bcd_to_byte(seconds);

    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_MINUTES, 1);
    *minutes = _ds1302_bcd_to_byte(_ds1302_read());
    _ds1302_transmission_end();

    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_HOUR, 1);
    *hour = _ds1302_bcd_to_byte(_ds1302_read());
    _ds1302_transmission_end();
}

void ds1302_set_time(uint8_t hour, uint8_t minutes) {
    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_MINUTES - 1, 0);
    _ds1302_write(_ds1302_byte_to_bcd(minutes), 0);
    _ds1302_transmission_end();

    _ds1302_transmission_begin();
    _ds1302_write(DS1302_REG_HOUR - 1, 0);
    _ds1302_write(_ds1302_byte_to_bcd(hour), 0);
    _ds1302_transmission_end();
}

void _ds1302_transmission_begin(void) {
    DS1302_PIN_CE = 0;
    DS1302_PIN_CLK = 0;
    DS1302_PIN_CE = 1;
    DS1302_CTRL_PIN_DATA = 0;
    __delay_us(4);
}

void _ds1302_transmission_end(void) {
    DS1302_PIN_CE = 0;
    __delay_us(4);
}

void _ds1302_write(uint8_t val, uint8_t is_data_req) {
    for (uint8_t i=0; i<8; i++) {
        DS1302_PIN_DATA_W = val & 1;
        __delay_us(1);
        DS1302_PIN_CLK = 1;
        __delay_us(1);

        if (i == 7 && is_data_req) {
            DS1302_CTRL_PIN_DATA = 1;
        }

        DS1302_PIN_CLK = 0;
        __delay_us(1);

        val >>= 1;
    }
}

uint8_t _ds1302_read() {
    uint16_t read_val = 0;

    for (uint8_t i=0; i<8; i++) {
        if (DS1302_PIN_DATA_R == 1) {
            read_val |= (1 << i);
        }
        DS1302_PIN_CLK = 1;
        __delay_us(1);
        DS1302_PIN_CLK = 0;
        __delay_us(1);
    }

    // TODO: debug why I have shifting of 1 bit in a result value
    return (uint8_t)((read_val >> 1) & 0xFF);
}

uint8_t _ds1302_bcd_to_byte(uint8_t data) {
	return ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

uint8_t _ds1302_byte_to_bcd(uint8_t data) {
	return ((data / 10) << 4) | (data % 10);
}