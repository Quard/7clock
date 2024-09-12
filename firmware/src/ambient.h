#ifndef __AMBIENT_H__
#define __AMBIENT_H__

#include <avr/io.h>


void ambient_init(void);
void ambient_start_measuring(void);
uint16_t ambient_get(void);

#endif
