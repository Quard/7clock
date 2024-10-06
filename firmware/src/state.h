#ifndef __STATE_H__
#define __STATE_H__

#include <avr/io.h>


#define STATE_MAX 5
#define EVENT_MAX 3


typedef enum {
  State_Idle = 0,
  State_SetupTimeMinuteL,
  State_SetupTimeMinuteH,
  State_SetupTimeHourL,
  State_SetupTimeHourH,
} State_t;

typedef enum {
  Button_Idle = 0,
  Button_ShortPress,
  Button_LongPress,
} ButtonState_t;


void state_clock_idle(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_minutes_l_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_minutes_l_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_minutes_h_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_minutes_h_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_hours_l_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_hours_l_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_hours_h_change(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);
void state_clock_setup_hours_h_apply(uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state);


static void (*const state_transition_table[STATE_MAX][EVENT_MAX]) (uint8_t *hours, uint8_t *minutes, State_t *state, ButtonState_t *btn_state) = {
  [State_Idle]             [Button_Idle]       = state_clock_idle,
  [State_Idle]             [Button_ShortPress] = state_clock_idle,
  [State_Idle]             [Button_LongPress]  = state_clock_setup,
  [State_SetupTimeMinuteL] [Button_Idle]       = state_clock_idle,
  [State_SetupTimeMinuteL] [Button_ShortPress] = state_clock_setup_minutes_l_change,
  [State_SetupTimeMinuteL] [Button_LongPress]  = state_clock_setup_minutes_l_apply,
  [State_SetupTimeMinuteH] [Button_Idle]       = state_clock_idle,
  [State_SetupTimeMinuteH] [Button_ShortPress] = state_clock_setup_minutes_h_change,
  [State_SetupTimeMinuteH] [Button_LongPress]  = state_clock_setup_minutes_h_apply,
  [State_SetupTimeHourL]   [Button_Idle]       = state_clock_idle,
  [State_SetupTimeHourL]   [Button_ShortPress] = state_clock_setup_hours_l_change,
  [State_SetupTimeHourL]   [Button_LongPress]  = state_clock_setup_hours_l_apply,
  [State_SetupTimeHourH]   [Button_Idle]       = state_clock_idle,
  [State_SetupTimeHourH]   [Button_ShortPress] = state_clock_setup_hours_h_change,
  [State_SetupTimeHourH]   [Button_LongPress]  = state_clock_setup_hours_h_apply,
};


#endif
