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

void LCD_print_string(const uint8_t *str)
{
    if(!LCD_was_initialized || str == NULL) LCD_error_handler();
    
    uint32_t index = 0;
    while(str[index] != '\0'){
        LCD_print_char(str[index]);
        index ++;
    }
}

void LCD_print_char(const uint8_t charact)
{
    if(!LCD_was_initialized) LCD_error_handler();
    LCD_PCF8574_hw_lcd_send_data(charact);
}

void LCD_clear(void)
{
  if(!LCD_was_initialized) LCD_error_handler();
  LCD_PCF8574_hw_send_command(LCD_DISP_CLEAR);
}

void LCD_show_cursor(bool_t show_cursor){

    if(!LCD_was_initialized) LCD_error_handler();
    if(show_cursor == true)
        LCD_PCF8574_hw_send_command(LCD_CURSOR_ON);
    else  LCD_PCF8574_hw_send_command(LCD_CURSOR_BLINK);
}

void LCD_show_cursos_blink(void)
{
    if(!LCD_was_initialized) LCD_error_handler();
    LCD_PCF8574_hw_send_command(LCD_CURSOR_BLINK);
}

uint8_t LCD_set_position_xy (uint8_t x, uint8_t y)
{
    if (x > LCD_COLS || y > LCD_ROWS)
    {
      return 1;
    }
    if (y == 0)
    {
      LCD_PCF8574_hw_send_command(LCD_POSITION | (LCD_ROW1_START + x));
    } else if (y == 1)
    {
      LCD_PCF8574_hw_send_command(LCD_POSITION | (LCD_ROW2_START + x));
    }
    return 0;
}

uint8_t LCD_shift_cursor (uint8_t item, uint8_t direction)
{
    if(!LCD_was_initialized) LCD_error_handler();

    if ((item != 'D') && (item != 'C'))
    {
      return 1;
    }
    if ((direction != 'R') && (direction != 'L'))
    {

      return 1;
    }
    if (item == 'C')
    {
      if (direction == LCD_RIGHT)
      {
        LCD_PCF8574_hw_send_command(LCD_SHIFT | LCD_CURSOR | LCD_RIGHT);
      } else {
        LCD_PCF8574_hw_send_command(LCD_SHIFT | LCD_CURSOR | LCD_LEFT);
      }
    } else
    {
      if (direction == 'R')
      {
        LCD_PCF8574_hw_send_command(LCD_SHIFT | LCD_DISPLAY | LCD_RIGHT);
      } else
      {
        LCD_PCF8574_hw_send_command(LCD_SHIFT | LCD_DISPLAY | LCD_LEFT);
      }
    }
    return 0;
}

void LCD_init(void *i2c_handler)
{
    if(LCD_was_initialized) return;
    LCD_PCF8574_hw_set_i2c_handler(i2c_handler);
    // Times sequence
    uint8_t time_seq []= {20 , 10, 1, 2};
    
    // Parameters sequence
    uint8_t params_seq [] = {
      LCD_4BIT_MODE | LCD_2_ROWS | LCD_FONT_5x8, // Function set: 4-bit mode, 2 lines, 5x8 dots
      LCD_DISP_OFF, // Display off
      LCD_RETURN_HOME,
      LCD_ENTRY_MODE,// Entry mode set: Increment cursor
      LCD_DISP_ON, // Display on, cursor off
      LCD_DISP_CLEAR // Display clear
    };
    uint8_t i = 0;
    for (i = 0; i < sizeof(time_seq) - 1; i++)
    {
      LCD_delay(time_seq[i]);
      LCD_PCF8574_hw_send_nibble(LCD_INIT_SEQ); // Wake up
    }
    LCD_PCF8574_hw_send_nibble(LCD_4BIT_MODE); // 4-bit mode
    
    for (uint8_t j = 0; j < sizeof(params_seq); j++) LCD_PCF8574_hw_send_command(params_seq[j]);
    LCD_delay(time_seq[i]);
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
