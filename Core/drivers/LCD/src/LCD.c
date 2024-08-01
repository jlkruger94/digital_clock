/*
 * LCD.c
 *
 *  Created on: Jul 31, 2024
 *      Author: José Luis Krüger
 */
#include "LCD.h"
#include "API_delay.h"
#include "LCD_PCF8574_hw.h"

// Private variables declaration
static bool_t LCD_was_initialized = false;

// Private function prototypes
static void LCD_delay(uint16_t);
static void LCD_error_handler(void);

void lcd_print_string(const uint8_t *str)
{
    if(!LCD_was_initialized || str == NULL) LCD_error_handler();
    
    uint32_t index = 0;
    while(str[index] != '\0'){
        lcd_print_char(str[index]);
        index ++;
    }
}

void lcd_print_char(const uint8_t charact)
{
    if(!LCD_was_initialized) LCD_error_handler();

    LCD_PCF8574_hw_lcd_send_data(charact);
    
}

void lcd_clear(void)
{
  if(!LCD_was_initialized) LCD_error_handler();

  LCD_PCF8574_hw_send_command(LCD_DISP_CLEAR);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    if(!LCD_was_initialized) LCD_error_handler();
}
void LCD_init(void)
{
    if(LCD_was_initialized) return;

    LCD_delay(16);
    LCD_PCF8574_hw_send_command(LCD_INIT_SEQ); // Wake up
    LCD_delay(5);
    LCD_PCF8574_hw_send_command(LCD_INIT_SEQ); // Wake up
    LCD_delay(1);
    LCD_PCF8574_hw_send_command(LCD_INIT_SEQ); // Wake up
    LCD_delay(10);
    LCD_PCF8574_hw_send_command(LCD_4BIT_MODE); // 4-bit mode
    LCD_delay(10);

    LCD_PCF8574_hw_send_command(LCD_4BIT_MODE | LCD_2_ROWS | LCD_FONT_5x8); // Function set: 4-bit mode, 2 lines, 5x8 dots
    LCD_PCF8574_hw_send_command(LCD_DISP_OFF); // Display off
    LCD_PCF8574_hw_send_command(LCD_DISP_CLEAR); // Clear display
    LCD_delay(2);
    LCD_PCF8574_hw_send_command(LCD_ENTRY_MODE); // Entry mode set: Increment cursor
    LCD_PCF8574_hw_send_command(LCD_DISP_ON); // Display on, cursor off

    LCD_was_initialized = true;
}

static void LCD_delay(uint16_t delay)
{
    if (delay == 0) return;

    static delay_t delay_struc = {
        .duration = 0,
        .running = false,
        .startTime = 0
    };

    DelayWrite(&delay_struc, delay);
    while(!DelayRead(&delay_struc));
}

static void LCD_error_handler(void)
{

}
