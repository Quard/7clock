#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <avr/io.h>


void twi_init(void);
void twi_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
void twi_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);


#endif
