#include "twi.h"


void twi_init(void) {
  PORTB.DIR &= ~(0b11 << 0);

  TWI0.MBAUD = 1;             // speed setup
  TWI0.MCTRLA |= TWI_ENABLE_bm; // enable I2C
  TWI0.MSTATUS |= 0x01;
}

void twi_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
  while ((TWI0.MSTATUS & 0b11) != 0x01);
  TWI0.MADDR = addr << 1;
  while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
  TWI0.MDATA = reg;
  while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
  TWI0.MADDR = addr << 1 | 0x01;
  while (len--) {
    while ((TWI0.MSTATUS & TWI_RIF_bm) == 0);
    *buf++ = TWI0.MDATA;
    TWI0.MCTRLB |=  len > 0 ? 0x02 : 0x03;
  }
}

void twi_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
  while ((TWI0.MSTATUS & 0b11) != 0x01);
  TWI0.MADDR = addr << 1;
  while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
  TWI0.MDATA = reg;
  while (len--) {
    while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
    TWI0.MDATA = *buf++;
  }
  while ((TWI0.MSTATUS & TWI_WIF_bm) == 0);
  TWI0.MCTRLB |= 0x03;  // send STOP
}
