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
void LCD_print_string(const uint8_t *str);
void LCD_clear(void);
void LCD_set_cursor(uint8_t row, uint8_t col);
void LCD_print_char(const uint8_t charact);
uint8_t LCD_shift_cursor (uint8_t item, uint8_t direction);
void LCD_show_cursor(uint8_t show_cursor);
void LCD_show_cursos_blink(void);
uint8_t LCD_set_position_xy (uint8_t x, uint8_t y);

#endif /* DRIVERS_LCD_INC_LCD_H_ */
