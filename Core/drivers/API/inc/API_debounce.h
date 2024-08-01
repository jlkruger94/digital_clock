/*
 * API_debounce.h
 *
 *  Created on: Jul 11, 2024
 *      Author: José Luis Krüger
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_
#include <stdbool.h>
#include <stdio.h>

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

/**
 * @brief Initializes the finite state machine (FSM) for button debouncing.
 */
void DebounceFSM_init(void);

/**
 * @brief Updates the state of the FSM by reading the button state and handling the transition logic.
 * Reads the current state of the button and performs state transitions based on the defined state diagram.
 */
void DebounceFSM_update(void);

/**
 * @brief Returns the state of the button (pressed or not) and resets the internal flag.
 * @return true if the button was pressed, false otherwise.
 */
bool ReadKey(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
