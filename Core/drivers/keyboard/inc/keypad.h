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

/**
 * @brief Initializes the keypad hardware and related settings.
 * 
 * This function initializes the keypad hardware and sets up the debounce delay.
 * 
 * @param keypad_hw Hardware configuration for the keypad.
 * 
 * @retval None
 */
void keypad_init(keyboard_hw_t keypad_hw);

/**
 * @brief Updates the keypad state based on button presses and releases.
 * 
 * This function manages the state machine for button debouncing and detection.
 * It transitions between states to correctly identify key presses and releases.
 * 
 * @retval None
 */
void keypad_update(void);

/**
 * @brief Retrieves the last pressed key.
 * 
 * This function returns the last key that was pressed and clears the record of the last pressed key.
 * 
 * @return The character of the last pressed key, or '\0' if no key was pressed.
 */
uint8_t keypad_get_key(void);

#endif /* DRIVERS_KEYBOARD_INC_KEYBOARD_H_ */
