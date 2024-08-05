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

void RTC_read(datetime_data_t *datetime);
void RTC_adjust(datetime_data_t *datetime);
void RTC_init(void *i2c_handler);

#endif /* DRIVERS_RTC_INC_RTC_H_ */
