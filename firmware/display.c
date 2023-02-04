#include "display.h"

inline void _display_write(uint8_t value, uint8_t idx);

volatile uint8_t _display_spi_sent;


const static uint8_t numbers[] = {
    //FGEDCBAx
    0b01000000,  // 0
    0b11110010,  // 1
    0b10001000,  // 2
    0b10100000,  // 3
    0b00110010,  // 4
    0b00100100,  // 5
    0b00000100,  // 6
    0b11110000,  // 7
    0b00000000,  // 8
    0b00100000,  // 9
};

void display_init(void) {
    SSPCON = 0x20;
    SSPSTAT = 0x0;
    SSP1IE = 1;
    RB5 = 1;
}

void display_show(uint8_t hours, uint8_t minutes, uint8_t mask) {
    if (mask & (1 << DISPLAY_MASK_MINUTE_L)) {
        _display_write(numbers[hours / 10], 0);
    }
    if (mask & (1 << DISPLAY_MASK_MINUTE_H)) {
        _display_write(numbers[hours % 10], 1);
    }
    if (mask & (1 << DISPLAY_MASK_HOUR_L)) {
        _display_write(numbers[minutes / 10], 2);
    }
    if (mask & (1 << DISPLAY_MASK_HOUR_H)) {
        _display_write(numbers[minutes % 10], 3);
    }
}

void display_show_load(void) {
    for (uint8_t idx=0; idx<4; idx++) {
        _display_write(0b10111111, idx);
    }
}

inline void _display_write(uint8_t value, uint8_t idx) {
    RB5 = 0;
    _display_spi_sent = 0;
    SSP1BUF = value;
    while (_display_spi_sent != 1);
    _display_spi_sent = 0;
    SSP1BUF = 0xFF & ~(1 << (1 + idx));
    while (_display_spi_sent != 1);
    _display_spi_sent = 0;
    RB5 = 1;
}
