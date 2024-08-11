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
static void RTC_error_handler(void);

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

/**
 * @brief  Initializes the RTC module with the provided I2C handler.
 * @param  i2c_handler: Pointer to the I2C handler.
 * @retval None
 */
void RTC_init(void *i2c_handler)
{
	if(i2c_handler == NULL) RTC_error_handler();

    RTC_DS3231_hw_set_I2C_handler(i2c_handler);
    RTC_was_initialized = true;
}

/**
 * @brief  Reads the current date and time from the RTC hardware and updates the datetime structure.
 * @param  datetime: Pointer to the datetime structure to be updated.
 * @retval None
 */
void RTC_read(datetime_data_t *datetime) {
    if(!RTC_was_initialized) RTC_error_handler();

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


/**
 * @brief  Sets the RTC hardware with the provided date and time values.
 * @param  datetime: Pointer to the datetime structure containing the values to be set.
 * @retval None
 */
void RTC_set(datetime_data_t *datetime) {
    if(!RTC_was_initialized || datetime == NULL) RTC_error_handler();;

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

/**
 * @brief  Validates and adjusts the datetime structure to ensure values are within acceptable ranges.
 * @param  datetime: Pointer to the datetime structure to be validated.
 * @retval None
 */
void RTC_validate_datetime(datetime_data_t *datetime)
{
	if(datetime == NULL) RTC_error_handler();

    // Ensure hours, minutes, and seconds are within valid ranges
    datetime->time.hours %= 24;
    datetime->time.minutes %= 60;
    datetime->time.seconds %= 60;

    // Ensure month is within valid range (1-12)
    datetime->date.month = (datetime->date.month == 0) ? 1 : datetime->date.month % 13;
    
    // Adjust year to valid range (0-99)
    datetime->date.year %= 100;

    // Calculate maximum days for the current month
    // Default value
    uint8_t max_days = 31;
    switch (datetime->date.month) {
        // April, June, September, November
        case 4: case 6: case 9: case 11:
            max_days = 30;
            break;
        // February
        case 2:
            if ((datetime->date.year % 4 == 0 && datetime->date.year % 100 != 0) || datetime->date.year % 400 == 0) {
                max_days = 29; // Leap year
            } else {
                max_days = 28;
            }
            break;
    }

    // Ensure day is within allowed range for the current month
    datetime->date.date = (datetime->date.date == 0) ? 1 : datetime->date.date;
    datetime->date.date = (datetime->date.date > max_days) ? max_days : datetime->date.date;
}

/**
 * @brief  Calculates the day of the week for the given date using Zeller's algorithm.
 * @param  datetime: Pointer to the datetime structure to be updated with the day of the week.
 * @retval None
 */
void RTC_calculate_day_of_week(datetime_data_t *datetime)
{
    uint8_t q = datetime->date.date;
    uint8_t m = datetime->date.month;
    uint16_t y = datetime->date.year;

    // Adjust month and year for January and February
    if (m == 1 || m == 2) {
        m += 12;
        y -= 1;
    }

    uint8_t K = y % 100;
    uint8_t J = y / 100;

    uint8_t h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;

    // Zeller's algorithm
    // 1 = Sunday,...,7 = Saturday:
    datetime->date.day = (h + 6) % 7 + 1;
}

/**
 * @brief  Error handler function that halts the system.
 * @retval None
 */
static void RTC_error_handler(void)
{
	while(1){
		__NOP();
	}
}
