/*
 * RTC_DS3231_hw.c
 *
 *  Created on: Aug 4, 2024
 *      Author: José Luis Krüger
 */
#include "RTC_DS3231_hw.h"

static I2C_HandleTypeDef *i2c_handler;
static const uint32_t MAX_TRANSMISSION_TIMEOUT = 15000;

static void RTC_DS3231_hw_delay(uint16_t delay);
static HAL_StatusTypeDef RTC_DS3231_hw_I2C_transmit(uint8_t *data, uint16_t size);
static HAL_StatusTypeDef RTC_DS3231_hw_I2C_receive(uint8_t *data, uint16_t size);

uint8_t RTC_DS3231_hw_read_register(uint8_t reg) {
    uint8_t data = 0;
    HAL_StatusTypeDef ret;

    ret = RTC_DS3231_hw_I2C_transmit(&reg, 1);
    if (ret != HAL_OK) {
        return 0;
    }

    ret = RTC_DS3231_hw_I2C_receive(&data, 1);
    if (ret != HAL_OK) {
        return 0;
    }

    return data;
}

void RTC_DS3231_hw_write_register(uint8_t reg, uint8_t value) {
    HAL_StatusTypeDef ret;
    uint8_t data[2] = {reg, value};

    ret = RTC_DS3231_hw_I2C_transmit(data, 2);
    if (ret != HAL_OK) {
        return;
    }
}

bool_t RTC_DS3231_hw_set_I2C_handler(void *handler) {
    if (handler == NULL) return false;
    i2c_handler = (I2C_HandleTypeDef *)handler;
    return true;
}

static HAL_StatusTypeDef RTC_DS3231_hw_I2C_transmit(uint8_t *data, uint16_t size) {
    HAL_StatusTypeDef ret;
    do {
        ret = HAL_I2C_Master_Transmit(i2c_handler, RTC_DS3231_ADDRESS << 1, data, size, MAX_TRANSMISSION_TIMEOUT);
    } while (ret == HAL_BUSY);

    return ret;
}

static HAL_StatusTypeDef RTC_DS3231_hw_I2C_receive(uint8_t *data, uint16_t size) {
    HAL_StatusTypeDef ret;
    do {
        ret = HAL_I2C_Master_Receive(i2c_handler, RTC_DS3231_ADDRESS << 1, data, size, MAX_TRANSMISSION_TIMEOUT);
    } while (ret == HAL_BUSY);

    return ret;
}

static void RTC_DS3231_hw_delay(uint16_t delay) {
    if (delay == 0) return;

    static delay_t delay_struc = {
        .duration = 0,
        .running = false,
        .startTime = 0
    };

    DelayWrite(&delay_struc, delay);
    while (!DelayRead(&delay_struc));
}
