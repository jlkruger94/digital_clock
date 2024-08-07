/*
 * keyboard.c
 *
 *  Created on: Aug 5, 2024
 *      Author: José Luis Krüger
 */

#include "keypad.h"

#define DEBOUNCE_TIME 50 // Tiempo de anti-rebote en ms

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

void keypad_init(keyboard_hw_t keypad_hw)
{
    keypad.state = BUTTON_UP;
    DelayInit(&debounce_delay, DEBOUNCE_TIME);
    last_key = '\0';
    BSP_LED_Init(LED1);
    keypad_init_hw(keypad_hw);
}

void keypad_update(void)
{
    switch (keypad.state)
    {
        case BUTTON_UP:
            // Iniciar el escaneo del teclado
            keypad.key = keypad_hw_scan();
            if (keypad.key != '\0')
            {
                keypad.state = BUTTON_FALLING;
            }
            break;

        case BUTTON_FALLING:
            if (DelayRead(&debounce_delay))
            {
                // Verificar si la tecla sigue presionada
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
                // Verificar si la tecla ha sido soltada
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

uint8_t keypad_get_key(void)
{
	uint8_t temp = last_pressed;
	last_pressed = '\0';
	return temp;
}
