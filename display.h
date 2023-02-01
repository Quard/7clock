#ifndef __DISPLAY_H__
#define	__DISPLAY_H__

#include "conf.h"

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif


void display_init(void);
void display_show(uint8_t hours, uint8_t minutes, uint8_t dot);
void display_show_load(void);


#ifdef	__cplusplus
}
#endif

#endif	/* __DISPLAY_H__ */

