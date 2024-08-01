/*
 * LCD.h
 *
 *  Created on: Jul 31, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_LCD_INC_LCD_H_
#define DRIVERS_LCD_INC_LCD_H_
#include "main.h"

void LCD_init(void);
void lcd_print_string(const uint8_t *str);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print_char(const uint8_t charact);

#endif /* DRIVERS_LCD_INC_LCD_H_ */
