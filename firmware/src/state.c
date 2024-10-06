#include "state.h"
#include "system.h"
#include "display.h"
#include "rtc.h"


static uint32_t tick;
static uint8_t display_mask = 0xFF;
static uint8_t digit_flashing;


void state_clock_idle(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  if (get_tick() - tick > 1000) {
    if (digit_flashing > 0) display_mask ^= (1 << (digit_flashing - 1));
    if (*state == State_Idle) rtc_get_datetime(hours, minutes);

    tick = get_tick();
  }

  display_show_clock(*hours, *minutes, display_mask);
}

void state_clock_setup(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *btn_state = Button_Idle;
  *state = State_SetupTimeMinuteL;
  digit_flashing = 4;
  display_mask = 0xFF;
}

void state_clock_setup_minutes_l_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  if (*minutes % 10 == 9) {
    *minutes = *minutes / 10 * 10;
  } else {
    *minutes += 1;
  }
  *btn_state = Button_Idle;
}

void state_clock_setup_minutes_l_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *btn_state = Button_Idle;
  *state = State_SetupTimeMinuteH;
  rtc_set_time(*hours, *minutes);
  digit_flashing = 3;
  display_mask = 0xFF;
}

void state_clock_setup_minutes_h_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *minutes += 10;
  if (*minutes > 60) {
    *minutes = *minutes % 10;
  }
  *btn_state = Button_Idle;
}

void state_clock_setup_minutes_h_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *btn_state = Button_Idle;
  *state = State_SetupTimeHourL;
  rtc_set_time(*hours, *minutes);
  digit_flashing = 2;
  display_mask = 0xFF;
}

void state_clock_setup_hours_l_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  if (*hours > 24) {
    *hours = 20;
  } else if (*hours % 10 == 9) {
    *hours = *hours / 10 * 10;
  } else {
        *hours += 1;
  }
  *btn_state = Button_Idle;
}

void state_clock_setup_hours_l_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *btn_state = Button_Idle;
  *state = State_SetupTimeHourH;
  rtc_set_time(*hours, *minutes);
  digit_flashing = 1;
  display_mask = 0xFF;
}

void state_clock_setup_hours_h_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *hours += 10;
  if (*hours >= 24) {
    *hours = *hours % 10;
  }
  *btn_state = Button_Idle;
}

void state_clock_setup_hours_h_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) {
  *btn_state = Button_Idle;
  *state = State_Idle;
  rtc_set_time(*hours, *minutes);
  digit_flashing = 0;
  display_mask = 0xFF;
}
