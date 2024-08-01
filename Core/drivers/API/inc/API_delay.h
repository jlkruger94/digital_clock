/*
 * API_delay.h
 *
 *  Created on: Jul 4, 2024
 *      Author: José Luis Krüger
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
typedef uint32_t tick_t;
typedef bool bool_t;

// delay structure
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

/**
 * @brief Initializes a delay structure with the specified duration.
 *
 * This function sets up a delay structure with the specified duration.
 * It validates the input pointer and sets the delay duration and the
 * running state to false.
 *
 * @param delay Pointer to the delay structure.
 * @param duration Duration of the delay in ticks.
 */
void DelayInit( delay_t * delay, tick_t duration );

/**
 * @brief Reads the current state of the delay.
 *
 * This function checks the state of the delay. If the delay is not running,
 * it starts the delay and returns false. If the delay is running and the
 * duration has elapsed, it stops the delay and returns true. Otherwise,
 * it returns false.
 *
 * @param delay Pointer to the delay structure.
 * @return true if the delay has elapsed, false otherwise.
 */
bool_t DelayRead( delay_t * delay );

/**
 * @brief Updates the duration of the delay.
 *
 * This function sets a new duration for the delay. It validates the input
 * pointer before updating the duration.
 *
 * @param delay Pointer to the delay structure.
 * @param duration New duration of the delay in ticks.
 */
void DelayWrite( delay_t * delay, tick_t duration );

/**
 * @brief Checks if the delay is currently running.
 *
 * This function returns the running state of the delay. It validates the
 * input pointer before returning the state.
 *
 * @param delay Pointer to the delay structure.
 * @return true if the delay is running, false otherwise.
 */
bool_t DelayIsRunning(delay_t * delay);

#endif /* API_INC_API_DELAY_H_ */
