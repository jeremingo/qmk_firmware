#include QMK_KEYBOARD_H
#include "../../../../../quantum/keymap_extras/keymap_dvorak.h"

void matrix_scan_user(void);

int cur_dance(tap_dance_state_t *state);
void uno_finished(tap_dance_state_t *state, void *user_data);
void uno_reset(tap_dance_state_t *state, void *user_data);

void register_config(void);
void unregister_config(void);

void register_arch(void);
void unregister_arch(void);

enum {
  SINGLE_TAP,
  DOUBLE_HOLD
};

enum {
  UNO
};

#define MODE_COUNT 2

const uint8_t COLORS[MODE_COUNT][3] = { { HSV_GREEN }, { HSV_RED } };

int mode = 0;

tap_dance_action_t tap_dance_actions[] = {
  [UNO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, uno_finished, uno_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  LAYOUT(TD(UNO))
};

const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM = { 2, 2, 2, 2 };

int uno_tap_state = 0;

uint16_t config_timer = 0xFFFF;

bool is_arch_open = false;

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom(HSV_GREEN);
}

int cur_dance(tap_dance_state_t *state) {
  return state->count == 2 && state->pressed ?  DOUBLE_HOLD :
    SINGLE_TAP;
}

void uno_finished(tap_dance_state_t *state, void *user_data) {
  uno_tap_state = cur_dance(state);

  switch (uno_tap_state) {
    case SINGLE_TAP: register_arch(); break;
    case DOUBLE_HOLD: register_config(); break;
  }
}

void uno_reset(tap_dance_state_t *state, void *user_data) {
  switch (uno_tap_state) {
    case SINGLE_TAP: unregister_arch(); break;
    case DOUBLE_HOLD: unregister_config(); break;
  }

  uno_tap_state = 0;
}

void register_config() {
  config_timer = timer_read();
  rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
}

void unregister_config() {
  config_timer = 0xFFFF;
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

void matrix_scan_user() {
  if (config_timer != 0xFFFF) {
    if ((timer_elapsed(config_timer) / 1500) % MODE_COUNT != mode) {
      mode = (mode + 1) % MODE_COUNT;
      rgblight_sethsv_noeeprom(COLORS[mode][0], COLORS[mode][1], COLORS[mode][2]);
    }
  }
}

void register_arch() {
  if (is_arch_open) {
    tap_code16(RCTL(DV_M));
  } else {
    tap_code16(LGUI(DV_R));
    wait_ms(200);
    
    tap_code(DV_A);
    tap_code(DV_R);
    tap_code(DV_C);
    tap_code(DV_H);
  }
}

void unregister_arch() {
  if (!is_arch_open) {
    wait_ms(170);
    tap_code(KC_ENTER);
  }
  
  is_arch_open = !is_arch_open;
}

