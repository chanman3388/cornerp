// Copyright 2022 LucW (@luc-languagetools)
// SPDX-License-Identifier: GPL-2.0-or-later


#include QMK_KEYBOARD_H

enum dasbob_layers {
    _WORKMAN,
    _LOWER,
    _RAISE,
    _SUPER,
    _FUNCTION
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_HOLD,
    TD_DOUBLE,
} td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TD_ALT_WIN,
    L1_SHFT,
    L2_CTRL,
};

td_state_t cur_dance(tap_dance_state_t *state);

void l1_finished(tap_dance_state_t *state, void *user_data);
void l1_reset(tap_dance_state_t *state, void *user_data);
void l2_finished(tap_dance_state_t *state, void *user_data);
void l2_reset(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_WIN] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_LGUI),
    [L1_SHFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l1_finished, l1_reset),
    [L2_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l2_finished, l2_reset),
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        return TD_SINGLE_HOLD;
    } else {
        return TD_DOUBLE;
    }

    return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'l1' tap dance.
static td_tap_t l1_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Create an instance of 'td_tap_t' for the 'l2' tap dance.
static td_tap_t l2_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void l1_finished(tap_dance_state_t *state, void *user_data) {
    l1_tap_state.state = cur_dance(state);
    switch (l1_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(1);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: register_code(KC_LSFT); break;
        default: break;
    }
}

void l1_reset(tap_dance_state_t *state, void *user_data) {
    switch (l1_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(1);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: unregister_code(KC_LSFT); break;
        default: break;
    }
    l1_tap_state.state = TD_NONE;
}

void l2_finished(tap_dance_state_t *state, void *user_data) {
    l2_tap_state.state = cur_dance(state);
    switch (l2_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(2);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: register_code(KC_LCTL); break;
        default: break;
    }
}

void l2_reset(tap_dance_state_t *state, void *user_data) {
    switch (l2_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(2);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: unregister_code(KC_LCTL); break;
        default: break;
    }
    l2_tap_state.state = TD_NONE;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_WORKMAN] = LAYOUT_split_3x6_3(
        KC_ESC,     KC_Q,           KC_D,       KC_R,           KC_W,           KC_B,           KC_J,           LT(_FUNCTION, KC_F),    KC_U,           KC_P,           KC_SCLN,        KC_BSPC,
        KC_LSFT,    KC_A,           KC_S,       KC_H,           KC_T,           KC_G,           KC_Y,           KC_N,                   KC_E,           KC_O,           KC_I,           KC_QUOT,
        KC_LCTL,    KC_Z,           KC_X,       KC_M,           KC_C,           KC_V,           KC_K,           KC_L,                   KC_COMM,        KC_DOT,         KC_SLSH,        XXXXXXX,
                                                TD(TD_ALT_WIN), TD(L1_SHFT),    LCTL_T(KC_SPC), LSFT_T(KC_ENT), TD(L2_CTRL),            KC_BSPC
    ),

    [_LOWER] = LAYOUT_split_3x6_3(
        KC_TRNS,    KC_1,           KC_2,       KC_3,           KC_4,           KC_5,           S(KC_1),        S(KC_2),                S(KC_3),        S(KC_4),        S(KC_5),        KC_DEL,
        KC_TRNS,    KC_6,           KC_7,       KC_8,           KC_9,           KC_0,           S(KC_6),        S(KC_7),                S(KC_8),        KC_QUOT,        KC_NUHS,        XXXXXXX,
        KC_TRNS,    KC_GRV,         KC_NUBS,    KC_MINS,        KC_EQL,         KC_NUHS,        S(KC_NUHS),     S(KC_EQL),              S(KC_MINS),     S(KC_NUBS),     KC_DEL,         XXXXXXX,
                                                KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,                KC_TAB
    ),

    [_RAISE] = LAYOUT_split_3x6_3(
        KC_TRNS,    XXXXXXX,        XXXXXXX,    KC_LBRC,        KC_RBRC,        XXXXXXX,        KC_QUOT,        KC_UP,                  S(KC_QUOT),     KC_HOME,        KC_PGUP,        XXXXXXX,
        KC_TRNS,    XXXXXXX,        XXXXXXX,    S(KC_9),        S(KC_0),        RALT(KC_4),     KC_LEFT,        KC_DOWN,                KC_RIGHT,       KC_END,         KC_PGDN,        XXXXXXX,
        KC_TRNS,    S(KC_NUHS),     KC_NUHS,    S(KC_LBRC),     S(KC_RBRC),     XXXXXXX,        XXXXXXX,        XXXXXXX,                XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,
                                                KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,                KC_ESC
    ),

    [_SUPER] = LAYOUT_split_3x6_3(
        KC_TRNS,    C(S(KC_ESC)),   XXXXXXX,    XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_PSCR,        XXXXXXX,                XXXXXXX,        XXXXXXX,        C(A(KC_DEL)),   XXXXXXX,
        KC_TRNS,    XXXXXXX,        XXXXXXX,    A(S(KC_TAB)),   A(KC_TAB),      XXXXXXX,        XXXXXXX,        C(G(KC_LEFT)),          C(G(KC_RIGHT)), XXXXXXX,        XXXXXXX,        XXXXXXX,
        KC_TRNS,    XXXXXXX,        XXXXXXX,    C(S(KC_TAB)),   C(KC_TAB),      XXXXXXX,        XXXXXXX,        C(S(KC_TAB)),           C(KC_TAB),      XXXXXXX,        XXXXXXX,        XXXXXXX,
                                                KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,                KC_TRNS
    ),

    [_FUNCTION] = LAYOUT_split_3x6_3(
        KC_TRNS,    XXXXXXX,        XXXXXXX,    XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_F1,          KC_F2,                  KC_F3,          KC_F4,          XXXXXXX,        XXXXXXX,
        KC_TRNS,    XXXXXXX,        XXXXXXX,    XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_F5,          KC_F6,                  KC_F7,          KC_F8,          XXXXXXX,        XXXXXXX,
        KC_TRNS,    XXXXXXX,        XXXXXXX,    XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_F9,          KC_F10,                 KC_F11,         KC_F12,         XXXXXXX,        XXXXXXX,
                                                KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,                KC_TRNS
    ),
};

#ifdef OLED_ENABLE

#define TEXT_MOVE_TIMEOUT 250
#define OLED_ROW_START 6
#define OLED_WRITEABLE_WIDTH 15
#define BUFF_LEN 16
#define START_COUNT 6
#define END_COUNT 4
#define NUM_QUOTES 4

static const char *quotes[] = {
    "Behind every cloud is a Bitcoin miner...",
    "Jono's mum's a nice lady!",
    "You miss every shot you don't take",
    "I'm quite a fan of this sausage"
};
static uint8_t quote_len[] = {40, 25, 34, 31};  // quote lendth without null terminator

char buff[BUFF_LEN];

static uint16_t anim_timer = 0;

typedef struct {
    uint8_t start_idx;
    uint8_t end_idx;
    uint8_t quote_id;
    uint8_t counter;
} quote_state;

static quote_state oled_quote_state = {
    0,
    0,
    0,
    0,
};

void render_text(void) {
    uint8_t end, i;
    if (oled_quote_state.start_idx >= quote_len[oled_quote_state.quote_id] && oled_quote_state.counter > 1) goto check_end;
    if (oled_quote_state.end_idx < quote_len[oled_quote_state.quote_id]) {
        end = OLED_WRITEABLE_WIDTH;
    } else {
        end = quote_len[oled_quote_state.quote_id] - oled_quote_state.start_idx;        
    }
    for (i = 0; i < end; i++) buff[i] = quotes[oled_quote_state.quote_id][oled_quote_state.start_idx + i];
    if (end < OLED_WRITEABLE_WIDTH) {
        for (i = end; i < OLED_WRITEABLE_WIDTH; i++) buff[i] = ' ';
    }
    if (end == 0) {
        for (i = 0; i < OLED_WRITEABLE_WIDTH; i++) buff[i] = ' ';
    }
    if (oled_quote_state.counter == 0 || end == 0) {
        oled_set_cursor(OLED_ROW_START, 3);
        oled_write_P(PSTR(buff), false);
    }
    if (oled_quote_state.start_idx == 0) {
        if (oled_quote_state.counter < START_COUNT) {
            oled_quote_state.counter++;
            goto skip;
        } else {
            oled_quote_state.counter = 0;
        }
    }
    oled_quote_state.start_idx++;
    oled_quote_state.end_idx++;
    if (oled_quote_state.start_idx >= quote_len[oled_quote_state.quote_id]) {
    check_end:
        oled_quote_state.counter++;
        if (oled_quote_state.counter <= END_COUNT) {
            goto skip;
        }
        oled_quote_state.counter = 0;
        oled_quote_state.start_idx = 0;
        oled_quote_state.end_idx = OLED_WRITEABLE_WIDTH;
        oled_quote_state.quote_id++;
        if (oled_quote_state.quote_id == NUM_QUOTES) {
            oled_quote_state.quote_id = 0;
        }
    }
skip:
    return;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    anim_timer = timer_read();
    buff[OLED_WRITEABLE_WIDTH] = '\0';
    oled_quote_state.start_idx = 0;
    oled_quote_state.end_idx = OLED_WRITEABLE_WIDTH;
    render_text();
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    }
    // return rotation;
    return OLED_ROTATION_0;
}

void draw_quotes(void) {
    static const char PROGMEM runqmk_logo[] = {0,0,0,0,152,152,152,152,152,152,24,24,24,
        152,152,24,24,24,152,152,24,24,152,152,24,24,24,152,152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,127,127,13,13,29,63,103,64,0,63,127,96,96,96,127,63,0,0,127,127,7,12,56,
        127,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,252,134,198,198,252,120,0,0,254,254,
        60,224,224,60,254,254,0,254,254,16,24,124,230,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,24,24,25,25,25,25,27,24,24,25,25,24,25,25,24,25,25,24,25,25,24,24,24,24,25,0,0,0};

        oled_write_raw_P(runqmk_logo, sizeof(runqmk_logo));

        // time for the next frame?
        if (timer_elapsed(anim_timer) > TEXT_MOVE_TIMEOUT) {
            anim_timer = timer_read();
            render_text();
        }
        oled_set_cursor(OLED_ROW_START, 2);
        oled_set_cursor(OLED_ROW_START, 0);
        oled_write_P(PSTR("by foostan"), false);
            oled_set_cursor(OLED_ROW_START, 1);
            oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _WORKMAN:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("workman\n"), false);
            break;
        case _LOWER:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("lower\n"), false);
            break;
        case _RAISE:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("raise\n"), false);
            break;
        case _SUPER:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("super\n"), false);
            break;
        case _FUNCTION:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("function\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_set_cursor(6, 1);
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master( )) {
        draw_quotes();
    } else {
        oled_render_logo();
     }
    return false;
}

#endif // OLED_ENABLE
