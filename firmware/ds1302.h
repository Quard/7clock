#ifndef __DS1302_H__
#define	__DS1302_H__

#include "conf.h"

#include <xc.h>

#define DS1302_PIN_CE         RA6
#define DS1302_PIN_CLK        RA0
#define DS1302_PIN_DATA_W     RA7
#define DS1302_PIN_DATA_R     LATA7
#define DS1302_CTRL_PIN_DATA  TRISA7

#ifdef	__cplusplus
extern "C" {
#endif

#define DS1302_REG_CH              0x81
#define DS1302_REG_TIMEDATE        0x80
#define DS1302_REG_TIMEDATE_BURST  0xBE
#define DS1302_REG_SECONDS         0x81
#define DS1302_REG_MINUTES         0x83
#define DS1302_REG_HOUR            0x85
#define DS1302_REG_WP              0x8E
    
typedef struct {
    unsigned pin_io_w;
    unsigned pin_io_r;
    unsigned pin_clk;
    unsigned pin_ce;
    unsigned ctrl_pin_io;
} DS1302_t;


uint8_t ds1302_is_enabled(void);
void ds1302_enable(void);
uint8_t ds1302_get_writeprotect(void);
void ds1302_get_time(uint8_t *hour, uint8_t *minutes);
void ds1302_set_time(uint8_t hour, uint8_t minutes);


#ifdef	__cplusplus
}
#endif

#endif	/* __DS1302_H__ */

