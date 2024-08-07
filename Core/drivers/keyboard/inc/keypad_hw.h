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

void keypad_init_hw(keyboard_hw_t);
uint8_t keypad_hw_scan(void);

#endif /* DRIVERS_KEYBOARD_INC_KEYBOARD_HW_H_ */
