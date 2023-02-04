#include "state_machine.h"


extern uint8_t min_brightness;
extern uint8_t display_mask;


void st_clock(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    if (*hour != 0xFF) {
        display_show(*hour, *minutes, display_mask);
    } else {
        display_show_load();
    }
}

void st_clock_min_brightness(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    min_brightness++;
    if (min_brightness > MIN_BRIGHTNESS) {
        min_brightness = 1;
    }

    *btn = ButtonState_Idle;
}

void st_clock_setup_ml(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    display_mask = 0b10001111;
    *state = State_SetupTimeMinutesL;
    *btn = ButtonState_Idle;
}

void st_clock_inc_ml(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    if (*minutes % 10 == 9) {
        *minutes = *minutes / 10 * 10;
    } else {
        *minutes += 1;
    }
    ds1302_set_time(*hour, *minutes);
    *btn = ButtonState_Idle;
}

void st_clock_setup_mh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    display_mask = 0b10001111;
    *state = State_SetupTimeMinutesH;
    *btn = ButtonState_Idle;
}

void st_clock_inc_mh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    *minutes += 10;
    if (*minutes > 60) {
        *minutes = *minutes % 10;
    }
    ds1302_set_time(*hour, *minutes);
    *btn = ButtonState_Idle;
}

void st_clock_setup_hl(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    display_mask = 0b10001111;
    *state = State_SetupTimeHourL;
    *btn = ButtonState_Idle;
}

void st_clock_inc_hl(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    if (*hour > 24) {
        *hour = 20;
    } else if (*hour % 10 == 9) {
        *hour = *hour / 10 * 10;
    } else {
        *hour += 1;
    }
    ds1302_set_time(*hour, *minutes);
    *btn = ButtonState_Idle;
}

void st_clock_setup_hh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    display_mask = 0b10001111;
    *state = State_SetupTimeHourH;
    *btn = ButtonState_Idle;
}

void st_clock_inc_hh(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    *hour += 10;
    if (*hour >= 24) {
        *hour = *hour % 10;
    }
    ds1302_set_time(*hour, *minutes);
    *btn = ButtonState_Idle;
}

void st_clock_setup_done(uint8_t *hour, uint8_t *minutes, State *state, ButtonState *btn) {
    display_mask = 0b10001111;
    *state = State_Clock;
    *btn = ButtonState_Idle;
}