/*
 * keypad.c
 *
 *  Created on: Aug 5, 2024
 *      Author: José Luis Krüger
 */

#include "keypad.h"

// Debounce time in milliseconds
#define DEBOUNCE_TIME 50

static keypad_t keypad = {
    .state              = BUTTON_UP,
    .current_row        = 0,
    .current_col        = 0,
    .key                = '\0',
    .key_pressed_flag   = false
    };
static uint8_t last_pressed = '\0';
static delay_t debounce_delay;
static uint8_t last_key;


/**
 * @brief Initializes the keypad hardware and related settings.
 * 
 * This function initializes the keypad hardware and sets up the debounce delay.
 * 
 * @param keypad_hw Hardware configuration for the keypad.
 * 
 * @retval None
 */
void keypad_init(keyboard_hw_t keypad_hw)
{
    keypad.state = BUTTON_UP;
    DelayInit(&debounce_delay, DEBOUNCE_TIME);
    last_key = '\0';
    keypad_init_hw(keypad_hw);
}

/**
 * @brief Updates the keypad state based on button presses and releases.
 * 
 * This function manages the state machine for button debouncing and detection.
 * It transitions between states to correctly identify key presses and releases.
 * 
 * @retval None
 */
void keypad_update(void)
{
    switch (keypad.state)
    {
        case BUTTON_UP:
            // Start scanning the keypad
            keypad.key = keypad_hw_scan();
            if (keypad.key != '\0')
            {
                keypad.state = BUTTON_FALLING;
            }
            break;

        case BUTTON_FALLING:
            if (DelayRead(&debounce_delay))
            {
                // Check if the key is still pressed
                if (keypad_hw_scan() == keypad.key)
                {
                    keypad.state = BUTTON_DOWN;
                    keypad.key_pressed_flag = true;
                    last_pressed = keypad.key;
                }
                else
                {
                    keypad.state = BUTTON_UP;
                }
            }
            break;

        case BUTTON_DOWN:
            if (keypad_hw_scan() != keypad.key)
            {
                keypad.state = BUTTON_RAISING;
            }
            break;

        case BUTTON_RAISING:
            if (DelayRead(&debounce_delay))
            {
                // Check if the key has been released
                if (keypad_hw_scan() != keypad.key)
                {
                    keypad.state = BUTTON_UP;
                }
                else
                {
                    keypad.state = BUTTON_DOWN;
                }
            }
            break;

        default:
            keypad.state = BUTTON_UP;
            break;
    }
}

/**
 * @brief Retrieves the last pressed key.
 * 
 * This function returns the last key that was pressed and clears the record of the last pressed key.
 * 
 * @return The character of the last pressed key, or '\0' if no key was pressed.
 */
uint8_t keypad_get_key(void)
{
	uint8_t temp = last_pressed;
	last_pressed = '\0';
	return temp;
}
