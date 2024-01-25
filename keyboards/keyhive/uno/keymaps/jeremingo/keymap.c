#include QMK_KEYBOARD_H
#include "../../../../../quantum/keymap_extras/keymap_dvorak.h"

void matrix_scan_user(void);

uint8_t cur_dance(tap_dance_state_t *state);
void uno_finished(tap_dance_state_t *state, void *user_data);
void uno_reset(tap_dance_state_t *state, void *user_data);

void register_config(void);
void unregister_config(void);

void register_arch(void);
void unregister_arch(void);

void register_enter(void);
void unregister_enter(void);

void register_rickroll(void);
void unregister_rickroll(void);

enum {
  SINGLE_TAP,
  DOUBLE_HOLD
};

enum {
  UNO
};

enum {
  ARCH,
  ENTER,
  RICKROLL
};

#define MODE_COUNT 3

typedef struct {
  uint8_t index;
  uint8_t color[3];
  void (*register_key)(void);
  void (*unregister_key)(void);
} mode;

const mode MODES[MODE_COUNT] = { 
  {
    .index = ARCH,
    .color = { HSV_GREEN },
    .register_key = register_arch,
    .unregister_key = unregister_arch
  },
  {
    .index = ENTER,
    .color = { HSV_RED },
    .register_key = register_enter,
    .unregister_key = unregister_enter
  },
  {
    .index = RICKROLL,
    .color = { HSV_ORANGE },
    .register_key = register_rickroll,
    .unregister_key = unregister_rickroll
  }
};

typedef union {
  uint32_t raw;
  struct {
    uint8_t mode_index;
  };
} user_config_t;

user_config_t user_config;

mode cur_mode;

tap_dance_action_t tap_dance_actions[] = {
  [UNO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, uno_finished, uno_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  LAYOUT(TD(UNO))
};

const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM = { 2, 2, 2, 2 };

uint8_t uno_tap_state = 0;

uint16_t config_timer = 0xFFFF;

bool is_arch_open = false;

void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  cur_mode = MODES[user_config.mode_index];
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom(cur_mode.color[0], cur_mode.color[1], cur_mode.color[2]);
}

uint8_t cur_dance(tap_dance_state_t *state) {
  return state->count == 2 && state->pressed ?  DOUBLE_HOLD :
    SINGLE_TAP;
}

void uno_finished(tap_dance_state_t *state, void *user_data) {
  uno_tap_state = cur_dance(state);

  switch (uno_tap_state) {
    case SINGLE_TAP: cur_mode.register_key(); break;
    case DOUBLE_HOLD: register_config(); break;
  }
}

void uno_reset(tap_dance_state_t *state, void *user_data) {
  switch (uno_tap_state) {
    case SINGLE_TAP: cur_mode.unregister_key(); break;
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

  if (user_config.mode_index != cur_mode.index) {
    user_config.mode_index = cur_mode.index;
    eeconfig_update_user(user_config.raw);
  }
}

void matrix_scan_user() {
  if (config_timer != 0xFFFF) {
    uint8_t wanted_index = (user_config.mode_index + (timer_elapsed(config_timer) / 1500) + 1)
      % MODE_COUNT;

    if (cur_mode.index != wanted_index) {
      cur_mode = MODES[wanted_index];
      rgblight_sethsv_noeeprom(cur_mode.color[0], cur_mode.color[1], cur_mode.color[2]);
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

void register_enter() {
  register_code(KC_ENTER);
}

void unregister_enter() {
  unregister_code(KC_ENTER);
}


void register_rickroll() {
    tap_code16(LGUI(KC_R));
    wait_ms(200);
    
    SEND_STRING("https://shattereddisk.github.io/rickroll/rickroll.mp4");
}

void unregister_rickroll() {
  wait_ms(170);
  tap_code(KC_ENTER);
}

