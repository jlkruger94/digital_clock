/*
 * keyboard.h
 *
 *  Created on: Aug 5, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_KEYBOARD_INC_KEYBOARD_H_
#define DRIVERS_KEYBOARD_INC_KEYBOARD_H_
#include "keypad_hw.h"
#include "API_delay.h"

typedef enum {
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RAISING
} keypad_state_t;

typedef struct {
    keypad_state_t state;
    uint8_t current_row;
    uint8_t current_col;
    char key;
    bool key_pressed_flag;
} keypad_t;

void keypad_init(keyboard_hw_t keypad_hw);
void keypad_update(void);
uint8_t keypad_get_key(void);

#endif /* DRIVERS_KEYBOARD_INC_KEYBOARD_H_ */
