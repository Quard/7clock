#ifndef __DISPLAY_H__
#define	__DISPLAY_H__

#include "conf.h"

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#define DISPLAY_MASK_MINUTE_L  0
#define DISPLAY_MASK_MINUTE_H  1
#define DISPLAY_MASK_HOUR_L    2
#define DISPLAY_MASK_HOUR_H    3
#define DISPLAY_MASK_DOT       7


void display_init(void);
void display_show(uint8_t hours, uint8_t minutes, uint8_t mask);
void display_show_load(void);


#ifdef	__cplusplus
}
#endif

#endif	/* __DISPLAY_H__ */

