/*
 * RTC.c
 *
 *  Created on: Aug 4, 2024
 *      Author: José Luis Krüger
 */


#include "RTC.h"

static bool_t RTC_was_initialized = false;

static uint8_t RTC_decode_BCD(uint8_t bin);
static uint8_t RTC_encode_BCD(uint8_t dec);

enum {
    SECOND,
    MINUTE,
    HOUR,
    DATE,
    DAY,
    MONTH,
    YEAR,
};

enum {
    REGISTER,
    DATA
};

void RTC_init(void *i2c_handler)
{
    RTC_DS3231_hw_set_I2C_handler(i2c_handler);
    RTC_was_initialized = true;
}

void RTC_read(datetime_data_t *datetime) {
    if(!RTC_was_initialized) return;

    uint8_t params[PARAMS_ARRAY_LENGTH][PARAMS_ARRAY_TYPES] = {
        [SECOND] = {RTC_DS3231_SECOND, 0},
        [MINUTE] = {RTC_DS3231_MINUTE, 0},
        [HOUR]   = {RTC_DS3231_HOUR,   0},
        [DATE]   = {RTC_DS3231_DATE,   0},
        [DAY]    = {RTC_DS3231_DAY,    0},
        [MONTH]  = {RTC_DS3231_MONTH,  0},
        [YEAR]   = {RTC_DS3231_YEAR,   0}
    };
    
    for (int i = 0; i < PARAMS_ARRAY_LENGTH; ++i) {
        params[i][DATA] = RTC_decode_BCD(RTC_DS3231_hw_read_register(params[i][REGISTER]));
    }

    datetime->time.seconds = params[SECOND][DATA];
    datetime->time.minutes = params[MINUTE][DATA];
    datetime->time.hours   = params[HOUR][DATA];
    datetime->date.date    = params[DATE][DATA];
    datetime->date.day     = params[DAY][DATA];
    datetime->date.month   = params[MONTH][DATA];
    datetime->date.year    = params[YEAR][DATA];
}

void RTC_adjust(datetime_data_t *datetime) {
    if(!RTC_was_initialized || datetime == NULL) return;

    uint8_t params[PARAMS_ARRAY_LENGTH][PARAMS_ARRAY_TYPES] = {
        [SECOND] = {RTC_DS3231_SECOND, datetime->time.seconds},
        [MINUTE] = {RTC_DS3231_MINUTE, datetime->time.minutes},
        [HOUR]   = {RTC_DS3231_HOUR,   datetime->time.hours},
        [DATE]   = {RTC_DS3231_DATE,   datetime->date.date},
        [DAY]    = {RTC_DS3231_DAY,    datetime->date.day},
        [MONTH]  = {RTC_DS3231_MONTH,  datetime->date.month},
        [YEAR]   = {RTC_DS3231_YEAR,   datetime->date.year}
    };

    for (int i = 0; i < PARAMS_ARRAY_LENGTH; ++i) {
        RTC_DS3231_hw_write_register(params[i][REGISTER], RTC_encode_BCD(params[i][DATA]));
    }
}

/**
 * @brief  Decodes a BCD (Binary Coded Decimal) value to its decimal equivalent.
 * @param  bin: The BCD value to decode (expected to be in the range 0x00 to 0x99).
 * @retval The decoded decimal value, or 0xFF if the input is invalid.
 */
static uint8_t RTC_decode_BCD(uint8_t bin)
{
	return (((bin & 0xF0) >> 4) * 10) + (bin & 0x0F);
}

/**
 * @brief  Encodes a decimal value to its BCD (Binary Coded Decimal) equivalent.
 * @param  dec: The decimal value to encode (expected to be in the range 0 to 99).
 * @retval The encoded BCD value, or 0xFF if the input is invalid.
 */
static uint8_t RTC_encode_BCD(uint8_t dec)
{
	return (dec % 10 + ((dec / 10) << 4));
}
