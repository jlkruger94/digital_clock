/*
 * keyboard_hw.h
 *
 *  Created on: Aug 5, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_KEYBOARD_INC_KEYBOARD_HW_H_
#define DRIVERS_KEYBOARD_INC_KEYBOARD_HW_H_
#include <stdint.h>
#include "main.h"

#define ROWS 4
#define COLS 4

typedef struct
{
   GPIO_TypeDef* GPIOx;
   uint16_t GPIO_Pin;

} GPIO_t;

typedef struct
{
    GPIO_t L1;
    GPIO_t L2;
    GPIO_t L3;
    GPIO_t L4;
    GPIO_t C1;
    GPIO_t C2;
    GPIO_t C3;
    GPIO_t C4;

} keyboard_hw_t;

/**
 * @brief Initializes the hardware for the keypad.
 * 
 * This function configures the GPIO pins for the rows as outputs and the columns as inputs,
 * based on the provided hardware configuration. Currently, GPIO initialization code is commented out.
 * 
 * @param hw Hardware configuration structure containing GPIO settings for rows and columns.
 * 
 * @retval None
 */
void keypad_init_hw(keyboard_hw_t);

/**
 * @brief Scans the keypad and returns the pressed key.
 * 
 * This function iterates through each row and column to detect which key is pressed.
 * It enables one row at a time and reads the column pins to find the pressed key.
 * 
 * @return The character corresponding to the pressed key, or '\0' if no key is pressed.
 */
uint8_t keypad_hw_scan(void);

#endif /* DRIVERS_KEYBOARD_INC_KEYBOARD_HW_H_ */
