/*
 * LCD_PCF8574_hw.c
 *
 *  Created on: Jul 31, 2024
 *      Author: José Luis Krüger
 */
#include "LCD_PCF8574_hw.h"
#include "main.h"
#include "API_delay.h"

extern I2C_HandleTypeDef hi2c1;
static const uint32_t MAX_TRANSMITION_TOUT = 15000;

static void LCD_PCF8574_hw_delay(uint16_t);
static void LCD_PCF8574_hw_I2C_transmit(uint8_t data);
static void LCD_PCF8574_hw_E_pulse (uint8_t data);
static void LCD_PCF8574_hw_send_byte(uint8_t data, uint8_t annex);

void LCD_PCF8574_hw_send_command(uint8_t cmd)
{
     LCD_PCF8574_hw_send_byte(cmd, LCD_PCF8574_P3);
}

void LCD_PCF8574_hw_lcd_send_data(uint8_t data)
{
    LCD_PCF8574_hw_send_byte(data, LCD_PCF8574_RS);
}

static void LCD_PCF8574_hw_send_byte(uint8_t data, uint8_t annex)
{

    
    uint8_t up_nibble = (data & 0xF0) | annex;
    uint8_t low_nibble = (data << 4) | annex;

    LCD_PCF8574_hw_send_nibble(up_nibble);
    LCD_PCF8574_hw_send_nibble(low_nibble);

}

void LCD_PCF8574_hw_send_nibble (uint8_t data)
{
    LCD_PCF8574_hw_I2C_transmit(data | LCD_PCF8574_E | LCD_PCF8574_P3);
    LCD_PCF8574_hw_delay(1);
    LCD_PCF8574_hw_I2C_transmit(data | LCD_PCF8574_P3);
    LCD_PCF8574_hw_delay(1);
}

static void LCD_PCF8574_hw_delay(uint16_t delay)
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

static void LCD_PCF8574_hw_E_pulse (uint8_t data)
{
  
  LCD_PCF8574_hw_I2C_transmit(data | LCD_PCF8574_E);
  
  LCD_PCF8574_hw_delay(1);
  
  LCD_PCF8574_hw_I2C_transmit(data);
  
  LCD_PCF8574_hw_delay(1);
}

static void LCD_PCF8574_hw_I2C_transmit(uint8_t data)
{
    HAL_I2C_Master_Transmit(&hi2c1,LCD_PCF8574_ADDRESS<<1,&data, sizeof(uint8_t),MAX_TRANSMITION_TOUT);
}
