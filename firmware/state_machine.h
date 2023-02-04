#ifndef __STATE_MACHINE_H__
#define	__STATE_MACHINE_H__

#include <xc.h>

#include "button.h"
#include "display.h"
#include "ds1302.h"


#define STATE_MAX 5
#define EVENT_MAX 3


typedef enum {
    State_Clock = 0,
    State_SetupTimeMinutesL,
    State_SetupTimeMinutesH,
    State_SetupTimeHourL,
    State_SetupTimeHourH,
} State;


void st_clock(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_min_brightness(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_setup_ml(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_inc_ml(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_setup_mh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_inc_mh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_setup_hl(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_inc_hl(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_setup_hh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_inc_hh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);
void st_clock_setup_done(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn);


void (*const state_transition_table[STATE_MAX][EVENT_MAX]) (uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) = {
    [State_Clock]              [ButtonState_Idle]        = st_clock,
    [State_Clock]              [ButtonState_ShortPress]  = st_clock_min_brightness,
    [State_Clock]              [ButtonState_LongPress]   = st_clock_setup_ml,
    [State_SetupTimeMinutesL]  [ButtonState_Idle]        = st_clock,
    [State_SetupTimeMinutesL]  [ButtonState_ShortPress]  = st_clock_inc_ml,
    [State_SetupTimeMinutesL]  [ButtonState_LongPress]   = st_clock_setup_mh,
    [State_SetupTimeMinutesH]  [ButtonState_Idle]        = st_clock,
    [State_SetupTimeMinutesH]  [ButtonState_ShortPress]  = st_clock_inc_mh,
    [State_SetupTimeMinutesH]  [ButtonState_LongPress]   = st_clock_setup_hl,
    [State_SetupTimeHourL]     [ButtonState_Idle]        = st_clock,
    [State_SetupTimeHourL]     [ButtonState_ShortPress]  = st_clock_inc_hl,
    [State_SetupTimeHourL]     [ButtonState_LongPress]   = st_clock_setup_hh,
    [State_SetupTimeHourH]     [ButtonState_Idle]        = st_clock,
    [State_SetupTimeHourH]     [ButtonState_ShortPress]  = st_clock_inc_hh,
    [State_SetupTimeHourH]     [ButtonState_LongPress]   = st_clock_setup_done,
};


#endif	/* __STATE_MACHINE_H__ */

