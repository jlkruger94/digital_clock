/*
 * keyboard_hw.c
 *
 *  Created on: Aug 5, 2024
 *      Author: José Luis Krüger
 */

#include "keypad_hw.h"

static keyboard_hw_t keyboard_hw;

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
void keypad_init_hw(keyboard_hw_t hw)
{
    keyboard_hw = hw;

/*    GPIO_InitTypeDef gpio_init_struct = {0};

    // Inicializar filas como salidas
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;

    gpio_init_struct.Pin = hw.L1.GPIO_Pin;
    HAL_GPIO_Init(hw.L1.GPIOx, &gpio_init_struct);
    HAL_GPIO_WritePin(hw.L1.GPIOx, hw.L1.GPIO_Pin, GPIO_PIN_RESET);
    gpio_init_struct.Pin = hw.L2.GPIO_Pin;
    HAL_GPIO_Init(hw.L2.GPIOx, &gpio_init_struct);
    HAL_GPIO_WritePin(hw.L2.GPIOx, hw.L2.GPIO_Pin, GPIO_PIN_RESET);
    gpio_init_struct.Pin = hw.L3.GPIO_Pin;
    HAL_GPIO_Init(hw.L3.GPIOx, &gpio_init_struct);
    HAL_GPIO_WritePin(hw.L3.GPIOx, hw.L3.GPIO_Pin, GPIO_PIN_RESET);
    gpio_init_struct.Pin = hw.L4.GPIO_Pin;
    HAL_GPIO_Init(hw.L4.GPIOx, &gpio_init_struct);
    HAL_GPIO_WritePin(hw.L4.GPIOx, hw.L4.GPIO_Pin, GPIO_PIN_RESET);
    // Inicializar columnas como entradas
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;
    gpio_init_struct.Pull = GPIO_NOPULL;

    gpio_init_struct.Pin = hw.C1.GPIO_Pin;
    HAL_GPIO_Init(hw.C1.GPIOx, &gpio_init_struct);

    gpio_init_struct.Pin = hw.C2.GPIO_Pin;
    HAL_GPIO_Init(hw.C2.GPIOx, &gpio_init_struct);

    gpio_init_struct.Pin = hw.C3.GPIO_Pin;
    HAL_GPIO_Init(hw.C3.GPIOx, &gpio_init_struct);

    gpio_init_struct.Pin = hw.C4.GPIO_Pin;
    HAL_GPIO_Init(hw.C4.GPIOx, &gpio_init_struct);*/
}

/**
 * @brief Scans the keypad and returns the pressed key.
 * 
 * This function iterates through each row and column to detect which key is pressed.
 * It enables one row at a time and reads the column pins to find the pressed key.
 * 
 * @return The character corresponding to the pressed key, or '\0' if no key is pressed.
 */
uint8_t keypad_hw_scan(void)
{
    uint8_t keypad_map[ROWS][COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    
    GPIO_t rows[ROWS] = {keyboard_hw.L1, keyboard_hw.L2, keyboard_hw.L3, keyboard_hw.L4};
    GPIO_t cols[COLS] = {keyboard_hw.C1, keyboard_hw.C2, keyboard_hw.C3, keyboard_hw.C4};
    
    for (uint8_t row = 0; row < ROWS; row++)
    {
        // Habilitar fila actual
        HAL_GPIO_WritePin(rows[row].GPIOx, rows[row].GPIO_Pin, GPIO_PIN_SET);

        for (uint8_t col = 0; col < COLS; col++)
        {

            // Leer el estado del pin de columna actual
            if (HAL_GPIO_ReadPin(cols[col].GPIOx, cols[col].GPIO_Pin) == GPIO_PIN_SET)
            {
                // Deshabilitar fila actual
                HAL_GPIO_WritePin(rows[row].GPIOx, rows[row].GPIO_Pin, GPIO_PIN_RESET);
                // Retornar el carácter correspondiente a la tecla presionada
                return keypad_map[row][col];
            }
        }
        // Deshabilitar fila actual
        HAL_GPIO_WritePin(rows[row].GPIOx, rows[row].GPIO_Pin, GPIO_PIN_RESET);
    }
    return '\0';
}
