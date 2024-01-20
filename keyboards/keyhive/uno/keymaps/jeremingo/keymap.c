#include QMK_KEYBOARD_H
#include "../../../../../quantum/keymap_extras/keymap_dvorak.h"

enum uno_keycode
{
  ARCH = SAFE_RANGE
};

enum layers
{
  _ARCH
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_ARCH] = LAYOUT(ARCH)
};

bool is_arch_open = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case ARCH:
      if (record->event.pressed) {
        if (is_arch_open) {
          tap_code16(RCTL(DV_M));
        } else {
          tap_code(KC_LGUI);
          wait_ms(100);
          
          tap_code(DV_A);
          tap_code(DV_R);
          tap_code(DV_C);
          tap_code(DV_H);
        }
      } else if (is_arch_open) {
        is_arch_open = false;
      } else {
        tap_code(KC_ENTER);
        is_arch_open = true;
      }
      break;
      return false;
  }
  return true;
}

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom(HSV_GREEN);
}

