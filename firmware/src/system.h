#ifndef __SYSTEM_H__
#define	__SYSTEM_H__

#include <avr/io.h>


void system_init(void);
uint32_t get_tick(void);
void system_btn_init(void);

#endif	/* __SYSTEM_H__ */
