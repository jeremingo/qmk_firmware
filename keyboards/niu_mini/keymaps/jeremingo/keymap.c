#include QMK_KEYBOARD_H
#include "../../../../quantum/keymap_extras/keymap_dvorak.h"

extern keymap_config_t keymap_config;

#define RAISE_SPC LT(_RAISE, KC_SPC)

enum tap_dances {
    TD_PLY_MUTE,
    TD_NXT_PRV
};

enum layers {
  _QWERTY,
  _DVORAK,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum keycodes {
  LAYOUT = SAFE_RANGE
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_PLY_MUTE]  = ACTION_TAP_DANCE_DOUBLE(KC_MPLY, KC_MUTE),
  [TD_NXT_PRV]  = ACTION_TAP_DANCE_DOUBLE(KC_MNXT, KC_MPRV)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_ortho_4x12(
  KC_TAB,  DV_QUOT,    DV_COMM,    DV_DOT,    DV_P,    DV_Y,    DV_F,    DV_G,    DV_C,    DV_R,    DV_L,    KC_BSPC,
  LCTL_T(KC_CAPS),  DV_A,    DV_O,    DV_E,    DV_U,    DV_I,    DV_D,    DV_H,    DV_T,    DV_N,    DV_S, DV_MINS,
  KC_LSFT, DV_SCLN,    DV_Q,    DV_J,    DV_K,    DV_X,    DV_B,    DV_M,    DV_W, DV_V,  DV_Z, KC_ENT ,
  KC_LGUI, KC_LCTL, RCTL_T(KC_ESC), KC_LALT, LSFT_T(KC_SPC), MO(_LOWER), MO(_RAISE),   RAISE_SPC,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

[_DVORAK] = LAYOUT_ortho_4x12(
  KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC,
  LCTL_T(KC_CAPS),  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
  KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT ,
  KC_LGUI, KC_LCTL, RCTL_T(KC_ESC), KC_LALT, LSFT_T(KC_SPC), MO(_LOWER), MO(_RAISE),   RAISE_SPC,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

[_LOWER] = LAYOUT_ortho_4x12(
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR,    KC_LPRN, KC_RPRN, KC_BSPC,
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS,    KC_LCBR, KC_RCBR, _______,
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_HOME, KC_END,  KC_PIPE, _______, _______,
  _______, _______, _______, _______, _______,     _______, _______,      _______,    KC_PSCR,    KC_BRID, KC_BRIU, LALT(KC_F4)
),

[_RAISE] = LAYOUT_ortho_4x12(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, _______,
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PGUP, KC_PGDN, KC_BSLS, _______, _______,
  _______, _______, _______, _______, _______,     _______, _______,      _______, TD(TD_NXT_PRV), KC_VOLD, KC_VOLU, TD(TD_PLY_MUTE)
),

[_ADJUST] = LAYOUT_ortho_4x12(
  LALT(LCTL(KC_DEL)), RESET,   DEBUG, _______,  _______,  _______, _______, _______, _______,_______, _______, KC_DEL ,
  _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, _______, _______, LAYOUT,  _______, _______,  _______,  _______,
  _______, _______,  _______,  _______,   _______,  _______,   _______,  _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,     _______, _______,      _______, _______, _______, _______, _______
)


};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LAYOUT:
      if (record->event.pressed) {
        switch (eeconfig_read_default_layer()) {
          case 1UL << _QWERTY:
            set_single_persistent_default_layer(_DVORAK);
            break;
          case 1UL << _DVORAK:
            set_single_persistent_default_layer(_QWERTY);
            break;
        }
      }
      return false;
      break;
  }
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case RAISE_SPC:
      return 200;
    default:
      return TAPPING_TERM;
  }
}
