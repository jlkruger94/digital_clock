/*
 * RTC_DS3231_hw.h
 *
 *  Created on: Aug 4, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_RTC_INC_RTC_DS3231_HW_H_
#define DRIVERS_RTC_INC_RTC_DS3231_HW_H_
#include <stdint.h>
#include "main.h"
#include "API_delay.h"

// Registers
#define RTC_DS3231_ADDRESS  0x68
#define RTC_DS3231_SECOND   0x00
#define RTC_DS3231_MINUTE   0x01
#define RTC_DS3231_HOUR     0x02
#define RTC_DS3231_DAY      0x03 // 1-7
#define RTC_DS3231_DATE     0x04 // 1-31
#define RTC_DS3231_MONTH    0x05 // 1-12 + Century
#define RTC_DS3231_YEAR     0x06 // 0-99

// Special Registers
#define RTC_DS3231_CTRL     0x0E
#define RTC_DS3231_STATUS   0x0F

/**
 * @brief  Sets the I2C handler for communication with the RTC_DS3231 device.
 * @param  handler: Pointer to the I2C handler.
 * @retval True if the handler was set successfully, false otherwise.
 */
bool_t RTC_DS3231_hw_set_I2C_handler(void *handler);

/**
 * @brief  Reads a register from the RTC_DS3231 device.
 * @param  reg: Register address to read from.
 * @retval The data read from the register.
 */
uint8_t RTC_DS3231_hw_read_register(uint8_t reg);

/**
 * @brief  Writes a value to a register in the RTC_DS3231 device.
 * @param  reg: Register address to write to.
 * @param  value: Value to write to the register.
 * @retval None
 */
void RTC_DS3231_hw_write_register(uint8_t reg, uint8_t value);

#endif /* DRIVERS_RTC_INC_RTC_DS3231_HW_H_ */
