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


bool_t RTC_DS3231_hw_set_I2C_handler(void *handler);

uint8_t RTC_DS3231_hw_read_register(uint8_t reg);

void RTC_DS3231_hw_write_register(uint8_t reg, uint8_t value);

#endif /* DRIVERS_RTC_INC_RTC_DS3231_HW_H_ */
