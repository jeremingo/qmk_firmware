// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_S,   KC_T,   KC_R,   KC_A,
        KC_O,   KC_I,   KC_Y,   KC_E,
                                            KC_J, KC_K
    )
};
