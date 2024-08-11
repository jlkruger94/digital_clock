/*
 * LCD_PCF8574_hw.c
 *
 *  Created on: Jul 31, 2024
 *      Author: José Luis Krüger
 */
#include "LCD_PCF8574_hw.h"
#include "main.h"
#include "API_delay.h"

static I2C_HandleTypeDef *i2c_handler;
static const uint32_t MAX_TRANSMITION_TOUT = 15000;

static void LCD_PCF8574_hw_delay(uint16_t);
static void LCD_PCF8574_hw_I2C_transmit(uint8_t data);
static void LCD_PCF8574_hw_send_byte(uint8_t data, uint8_t annex);

/**
 * @brief Sets the I2C handler for communication with the LCD.
 * 
 * @param handler Pointer to the I2C handler.
 *        Must not be NULL.
 * 
 * @return true if the I2C handler was set successfully.
 * @return false if the handler is NULL.
 */
bool_t LCD_PCF8574_hw_set_i2c_handler(void *handler)
{
    if(handler == NULL) return false;
    i2c_handler = (I2C_HandleTypeDef *)handler;
    return true;
}

/**
 * @brief Sends a command to the LCD.
 * 
 * @param cmd The command byte to be sent.
 * 
 * @retval None
 */
void LCD_PCF8574_hw_send_command(uint8_t cmd)
{
     LCD_PCF8574_hw_send_byte(cmd, LCD_PCF8574_P3);
}

/**
 * @brief Sends data to the LCD.
 * 
 * @param data The data byte to be sent.
 * 
 * @retval None
 */
void LCD_PCF8574_hw_lcd_send_data(uint8_t data)
{
    LCD_PCF8574_hw_send_byte(data, LCD_PCF8574_RS);
}

/**
 * @brief Sends a byte to the LCD in two nibbles.
 * 
 * @param data The byte to be sent.
 * @param annex The control byte (either RS or P3) to be combined with the data.
 * 
 * @retval None
 */
static void LCD_PCF8574_hw_send_byte(uint8_t data, uint8_t annex)
{
    uint8_t up_nibble = (data & 0xF0) | annex;
    uint8_t low_nibble = (data << 4) | annex;

    LCD_PCF8574_hw_send_nibble(up_nibble);
    LCD_PCF8574_hw_send_nibble(low_nibble);
}

/**
 * @brief Sends a nibble to the LCD.
 * 
 * @param data The nibble to be sent.
 * 
 * @retval None
 */
void LCD_PCF8574_hw_send_nibble (uint8_t data)
{
    LCD_PCF8574_hw_I2C_transmit(data | LCD_PCF8574_E | LCD_PCF8574_P3);
    LCD_PCF8574_hw_delay(1);
    LCD_PCF8574_hw_I2C_transmit(data | LCD_PCF8574_P3);
    LCD_PCF8574_hw_delay(1);
}


/**
 * @brief Delays execution for a specified amount of time.
 * 
 * @param delay Duration of the delay in milliseconds.
 * 
 * @retval None
 */
static void LCD_PCF8574_hw_delay(uint16_t delay)
{
    if (delay == 0) return;

    static delay_t delay_struct = {
        .duration = 0,
        .running = false,
        .startTime = 0
    };

    DelayWrite(&delay_struct, delay);
    while(!DelayRead(&delay_struct));
}

/**
 * @brief Transmits a byte of data over I2C.
 * 
 * @param data The byte of data to be transmitted.
 * 
 * @retval None
 */
static void LCD_PCF8574_hw_I2C_transmit(uint8_t data)
{
    HAL_I2C_Master_Transmit(i2c_handler,LCD_PCF8574_ADDRESS<<1,&data, sizeof(uint8_t),MAX_TRANSMITION_TOUT);
}