/*
 * API_debounce.c
 *
 *  Created on: Jul 11, 2024
 *      Author: José Luis Krüger
 *
 */

#include "API_debounce.h"
#include "API_delay.h"
#include "API_uart.h"

static debounceState_t currentState;
static bool_t buttonPressedFlag;
static delay_t debounceDelay;
static const int DEBOUNCE_DELAY = 40;

// Private function prototypes
static void ButtonPressed();
static void ButtonReleased();
static void DebounceErrorHandler(void);

/**
 * @brief Initializes the finite state machine (FSM) for button debouncing.
 */
void DebounceFSM_init(void)
{
    currentState = BUTTON_UP;
    DelayInit(&debounceDelay, DEBOUNCE_DELAY);
    buttonPressedFlag = false;
    BSP_LED_Init(LED1);
}

/**
 * @brief Updates the state of the FSM by reading the button state and handling the transition logic.
 * Reads the current state of the button and performs state transitions based on the defined state diagram.
 */
void DebounceFSM_update(void)
{
	uint8_t buffer[256] = { 0 };
    uint32_t currentButtonState = BSP_PB_GetState(BUTTON_USER);

    switch (currentState)
    {
        case BUTTON_UP:
            if (currentButtonState == GPIO_PIN_SET)
            {
                currentState = BUTTON_FALLING;
                DelayRead(&debounceDelay);
                sprintf((char*)buffer,"\n\rFlanco ascendente detectado");
                uartSendString(buffer);
            }
            break;
        case BUTTON_FALLING:
            if (DelayRead(&debounceDelay))
            {
                if (currentButtonState == GPIO_PIN_SET)
                {
                    currentState = BUTTON_DOWN;
                    ButtonPressed();
                    buttonPressedFlag = true;
                } else
                {
                    currentState = BUTTON_UP;
                }
            }
            break;
        case BUTTON_DOWN:
            if (currentButtonState == GPIO_PIN_RESET)
            {
                currentState = BUTTON_RAISING;
                DelayRead(&debounceDelay);
                sprintf((char*)buffer,"\n\rFlanco descendente detectado");
                uartSendString(buffer);
            }
            break;
        case BUTTON_RAISING:
            if (DelayRead(&debounceDelay))
            {
                if (currentButtonState == GPIO_PIN_RESET) {
                    currentState = BUTTON_UP;
                    ButtonReleased();
                } else
                {
                    currentState = BUTTON_DOWN;
                }
            }
            break;
        default:
            DebounceErrorHandler();
            break;
    }
}

/**
 * @brief Returns the state of the button (pressed or not) and resets the internal flag.
 * @return true if the button was pressed, false otherwise.
 */
bool_t ReadKey(void)
{
    if (buttonPressedFlag)
    {
        buttonPressedFlag = false;
        return true;
    }
    return false;
}

/**
 * @brief Function called when the button is detected as pressed.
 * Turns on the associated LED.
 */
static void ButtonPressed(void)
{
    BSP_LED_On(LED1);
}

/**
 * @brief Function called when the button is detected as released.
 * Turns off the associated LED.
 */
static void ButtonReleased(void)
{
    BSP_LED_Off(LED1);
}

/**
 * @brief Handles errors in the debounce FSM.
 * Turns on an error LED and enters an infinite loop.
 */
static void DebounceErrorHandler(void)
{
    BSP_LED_On(LED3);
    while (1)
    {
        __NOP();
    }
}


