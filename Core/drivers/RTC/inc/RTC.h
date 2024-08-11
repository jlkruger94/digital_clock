/*
 * RTC.h
 *
 *  Created on: Aug 4, 2024
 *      Author: José Luis Krüger
 */

#ifndef DRIVERS_RTC_INC_RTC_H_
#define DRIVERS_RTC_INC_RTC_H_
#include "RTC_DS3231_hw.h"

#define PARAMS_ARRAY_LENGTH 7
#define PARAMS_ARRAY_TYPES 2

typedef struct { 
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    } time_data_t;

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t date;
    uint8_t day;
    } date_data_t;

typedef struct {
    time_data_t time;
    date_data_t date;
    } datetime_data_t;

/**
 * @brief  Initializes the RTC module with the provided I2C handler.
 * @param  i2c_handler: Pointer to the I2C handler.
 * @retval None
 */
void RTC_init(void *i2c_handler);

/**
 * @brief  Reads the current date and time from the RTC hardware and updates the datetime structure.
 * @param  datetime: Pointer to the datetime structure to be updated.
 * @retval None
 */
void RTC_read(datetime_data_t *datetime);

/**
 * @brief  Sets the RTC hardware with the provided date and time values.
 * @param  datetime: Pointer to the datetime structure containing the values to be set.
 * @retval None
 */
void RTC_set(datetime_data_t *datetime);

/**
 * @brief  Validates and adjusts the datetime structure to ensure values are within acceptable ranges.
 * @param  datetime: Pointer to the datetime structure to be validated.
 * @retval None
 */
void RTC_validate_datetime(datetime_data_t *datetime);

/**
 * @brief  Calculates the day of the week for the given date using Zeller's algorithm.
 * @param  datetime: Pointer to the datetime structure to be updated with the day of the week.
 * @retval None
 */
void RTC_calculate_day_of_week(datetime_data_t *datetime);

#endif /* DRIVERS_RTC_INC_RTC_H_ */
