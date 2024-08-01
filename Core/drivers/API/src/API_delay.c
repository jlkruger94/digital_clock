/*
 * API_delay.c
 *
 *  Created on: Jul 4, 2024
 *      Author: José Luis Krüger
 */

#include "API_delay.h"

// Private function prototypes
static void DelayErrorHandler(void);

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
void DelayInit(delay_t *delay, tick_t duration)
{
    // Validate that a pointer is received as a parameter
    if (delay == NULL)
        DelayErrorHandler();
    // The structure is initialized with the delay values
    delay->duration = duration;
    delay->running = false;
}

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
bool_t DelayRead(delay_t *delay)
{
    // Validate that a pointer is received as a parameter
    if (delay == NULL)
        DelayErrorHandler();

    if (delay->running != true)
    {
        delay->running = true;
        delay->startTime = HAL_GetTick();
        return false;
    } else if (HAL_GetTick() - delay->startTime >= delay->duration)
    {
        delay->running = false;
        return true;
    } else
    {
        return false;
    }
}

/**
 * @brief Updates the duration of the delay.
 *
 * This function sets a new duration for the delay. It validates the input
 * pointer before updating the duration.
 *
 * @param delay Pointer to the delay structure.
 * @param duration New duration of the delay in ticks.
 */
void DelayWrite(delay_t *delay, tick_t duration)
{
    // Validate that a pointer is received as a parameter
    if (delay == NULL)
        DelayErrorHandler();

    delay->duration = duration;
}

/**
 * @brief Checks if the delay is currently running.
 *
 * This function returns the running state of the delay. It validates the
 * input pointer before returning the state.
 *
 * @param delay Pointer to the delay structure.
 * @return true if the delay is running, false otherwise.
 */
bool_t DelayIsRunning(delay_t *delay)
{
    // Validate that a pointer is received as a parameter
    if (delay == NULL)
        DelayErrorHandler();
    return delay->running;
}

/**
 * @brief Error handler for invalid delay operations.
 *
 * This function is called when an invalid operation is detected in the
 * delay functions (e.g., a null pointer is passed). It sets an error
 * indicator (e.g., lights an LED) and enters an infinite loop.
 */
static void DelayErrorHandler(void)
{
	BSP_LED_On(LED3);
    while (1) {
        __NOP();
    }
}

