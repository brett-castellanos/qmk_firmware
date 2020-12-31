#include QMK_KEYBOARD_H

// Layers
#define _MAC 0
#define _FN 1
#define _LEFT 2
#define _RIGHT 3

// Custom Keycodes
#define GUI_CTL TD(TD_GUI_CTL)
#define F_LEFT TD(TD_F_LEFT)
#define J_RGHT TD(TD_J_RIGHT)
#define ESC_MEH TD(TD_ESC_MEH)

// Tap Dance declarations
enum td_keycodes {
    TD_GUI_CTL,
    TD_F_LEFT,
    TD_J_RIGHT,
    TD_ESC_MEH
};

// Tap Dance states
typedef enum {
    SINGLE_TAP,
    SINGLE_HOLD,
    DOUBLE_SINGLE_TAP,
    DOUBLE_TAP,
    DOUBLE_HOLD
} td_state_t;

// Create a global instance of the Tap Dance state type
static td_state_t td_state;

// Tap Dance functions
uint8_t cur_dance(qk_tap_dance_state_t *state); // Gets current tap dance state

// `finished` and `reset` functions for each tapdance keycode
void guictrl_finished(qk_tap_dance_state_t *state, void *user_data);
void guictrl_reset(qk_tap_dance_state_t *state, void *user_data);

void fleft_finished(qk_tap_dance_state_t *state, void *user_data);
void fleft_reset(qk_tap_dance_state_t *state, void *user_data);

void jright_finished(qk_tap_dance_state_t *state, void *user_data);
void jright_reset(qk_tap_dance_state_t *state, void *user_data);

void escmeh_finished(qk_tap_dance_state_t *state, void *user_data);
void escmeh_reset(qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_MAC] = LAYOUT_all(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_BSPC,
        KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        ESC_MEH,          KC_A,    KC_S,    KC_D,    F_LEFT,  KC_G,    KC_H,    J_RGHT,  KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_DEL,
        KC_LCTL, KC_LALT, KC_LGUI,                   KC_SPC,  KC_SPC,  KC_SPC,                    GUI_CTL, MO(_FN), KC_LEFT, KC_DOWN, KC_RIGHT
    ),

    [_FN] = LAYOUT_all(
	    RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_DEL,
        _______,          RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______,                   _______, _______, _______,                   _______, _______, _______, _______, _______
    ),

    [_LEFT] = LAYOUT_all(
	    RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_DEL,
        _______,          RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLU, _______,
        _______, _______, _______,                   _______, _______, _______,                   _______, _______, _______, KC_VOLD, _______
    ),

    [_RIGHT] = LAYOUT_all(
	    RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_DEL,
        _______,          RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______,                   _______, _______, _______,                   _______, _______, _______, _______, _______
    )
};

// Determine the tapdance state to return
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    }
    if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    else return 3; // Any number higher than the maximum state value you return above
}

// Handle the possible states for each Tap Dance keycode

void guictrl_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            register_mods(MOD_BIT(KC_RGUI));
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            register_mods(MOD_BIT(KC_RCTL));
            break;
    }
}

void guictrl_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_RGUI));
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_mods(MOD_BIT(KC_RCTL));
            break;

    }
}

void fleft_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
            register_code16(KC_F);
            break;
        case SINGLE_HOLD:
            layer_on(_LEFT);
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            tap_code(KC_F);
            register_code16(KC_F);
            break;
    }
}

void fleft_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
            unregister_code16(KC_F);
            break;
        case SINGLE_HOLD:
            layer_off(_LEFT);
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_code16(KC_F);
            break;
    }
}

void jright_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
            register_code16(KC_J);
            break;
        case SINGLE_HOLD:
            layer_on(_RIGHT);
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            tap_code(KC_J);
            register_code16(KC_J);
            break;
    }
}

void jright_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
            unregister_code16(KC_J);
            break;
        case SINGLE_HOLD:
            layer_off(_RIGHT);
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_code16(KC_J);
            break;
    }
}

void escmeh_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
            register_code16(KC_ESC);
            break;
        case SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LCTL));
            register_mods(MOD_BIT(KC_LSFT));
            register_mods(MOD_BIT(KC_LALT));
            register_mods(MOD_BIT(KC_LGUI));
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            break;
    }
}

void escmeh_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
            unregister_code16(KC_ESC);
            break;
        case SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LCTL));
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_mods(MOD_BIT(KC_LALT));
            unregister_mods(MOD_BIT(KC_LGUI));
            break;
        case DOUBLE_SINGLE_TAP:
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            break;
    }
}


// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_GUI_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, guictrl_finished, guictrl_reset),
    [TD_F_LEFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fleft_finished, fleft_reset),
    [TD_J_RIGHT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, jright_finished, jright_reset),
    [TD_ESC_MEH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, escmeh_finished, escmeh_reset)
};
