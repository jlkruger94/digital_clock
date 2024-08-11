/*
 * LCD.h
 *
 *  Created on: Jul 31, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_LCD_INC_LCD_H_
#define DRIVERS_LCD_INC_LCD_H_
#include "main.h"
#include <stdbool.h>

typedef bool bool_t;

/**
 * @brief Initializes the LCD, setting up the I2C handler and sending initialization commands.
 * 
 * @param i2c_handler Pointer to the I2C handler.
 *        Must not be NULL.
 * 
 * @retval None
 */
void LCD_init(void *i2c_handler);

/**
 * @brief Prints a null-terminated string to the LCD.
 * 
 * @param str Pointer to the string to be printed.
 *        Must not be NULL.
 * 
 * @retval None
 */
void LCD_print_string(const uint8_t *str);

/**
 * @brief Clears the LCD display.
 * 
 * @retval None
 */
void LCD_clear(void);

/**
 * @brief Prints a single character to the LCD.
 * 
 * @param charact The character to be printed.
 * 
 * @retval None
 */
void LCD_print_char(const uint8_t charact);

/**
 * @brief Shifts the cursor or display content.
 * 
 * @param item Indicates whether to shift the cursor ('C') or display ('D').
 * @param direction Indicates the direction of the shift ('R' for right, 'L' for left).
 * 
 * @return 0 if successful.
 * @retval 1 if invalid arguments.
 */
uint8_t LCD_shift_cursor (uint8_t item, uint8_t direction);

/**
 * @brief Shows or hides the cursor on the LCD.
 * 
 * @param show_cursor Boolean value indicating whether to show the cursor.
 *        True to show, false to hide.
 * 
 * @retval None
 */
void LCD_show_cursor(bool_t show_cursor);

/**
 * @brief Makes the cursor blink on the LCD.
 * 
 * @retval None
 */
void LCD_show_cursor_blink(void);

/**
 * @brief Sets the cursor position on the LCD.
 * 
 * @param x The column position (0 to LCD_COLS-1).
 * @param y The row position (0 for the first row, 1 for the second row).
 * 
 * @return 0 if successful.
 * @retval 1 if the position is out of bounds.
 */
uint8_t LCD_set_position_xy (uint8_t x, uint8_t y);

#endif /* DRIVERS_LCD_INC_LCD_H_ */
