/*
 * LCD_PCF8574_hw.h
 *
 *  Created on: Jul 31, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_LCD_INC_LCD_PCF8574_HW_H_
#define DRIVERS_LCD_INC_LCD_PCF8574_HW_H_
#include <stdint.h>

#define LCD_PCF8574_ADDRESS      0x27
#define LCD_PCF8574_RS       0x01
#define LCD_PCF8574_RW       0x02
#define LCD_PCF8574_E        0x04
#define LCD_PCF8574_P3       0x08
#define LCD_PCF8574_DB4      0x10
#define LCD_PCF8574_DB5      0x20
#define LCD_PCF8574_DB6      0x40
#define LCD_PCF8574_DB7      0x80
#define LCD_BUSY_FLAG    LCD_DB7
#define LCD_INIT_SEQ     0x30
#define LCD_DISP_CLEAR   0x01
#define LCD_DISP_OFF     0x08
#define LCD_DISP_ON      0x0C
#define LCD_CURSOR_ON    0x0E
#define LCD_CURSOR_BLINK 0x0F
#define LCD_RETURN_HOME  0x02
#define LCD_ENTRY_MODE   0x06
#define LCD_4BIT_MODE    0x20
#define LCD_8BIT_MODE    0x30
#define LCD_2_ROWS       0x08
#define LCD_FONT_5x8     0x00
#define LCD_FONT_5x10    0x04
#define LCD_POSITION     0x80
#define LCD_SHIFT        0x10
#define LCD_CURSOR       0x00
#define LCD_DISPLAY      0x08
#define LCD_LEFT         0x00
#define LCD_RIGHT        0x04

#define LCD_ROWS         2
#define LCD_COLS         16
#define LCD_ROW1_START   0x00
#define LCD_ROW1_END     LCD_COLS
#define LCD_ROW2_START   0x40
#define LCD_ROW2_END     LCD_COLS

void LCD_PCF8574_hw_send_command(uint8_t cmd);
void LCD_PCF8574_hw_lcd_send_data(uint8_t data);


#endif /* DRIVERS_LCD_INC_LCD_PCF8574_HW_H_ */
