/*
 * RTC_DS3231_hw.c
 *
 *  Created on: Aug 4, 2024
 *      Author: José Luis Krüger
 */
#include "RTC_DS3231_hw.h"
#define MAX_DELAY (MAX_TRANSMISSION_TIMEOUT * COMM_RETRIES)

static I2C_HandleTypeDef *i2c_handler;
static const uint32_t MAX_TRANSMISSION_TIMEOUT = 1500;
static const uint32_t COMM_RETRIES = 5;

static HAL_StatusTypeDef RTC_DS3231_hw_I2C_transmit(uint8_t *data, uint16_t size);
static HAL_StatusTypeDef RTC_DS3231_hw_I2C_receive(uint8_t *data, uint16_t size);
static void RTC_DS3231_hw_error_handler(void);

/**
 * @brief  Reads a register from the RTC_DS3231 device.
 * @param  reg: Register address to read from.
 * @retval The data read from the register.
 */
uint8_t RTC_DS3231_hw_read_register(uint8_t reg) {
    uint8_t data = 0;
    HAL_StatusTypeDef ret;

    ret = RTC_DS3231_hw_I2C_transmit(&reg, 1);
    if (ret != HAL_OK) {
        RTC_DS3231_hw_error_handler();
    }

    ret = RTC_DS3231_hw_I2C_receive(&data, 1);
    if (ret != HAL_OK) {
        RTC_DS3231_hw_error_handler();
    }

    return data;
}

/**
 * @brief  Writes a value to a register in the RTC_DS3231 device.
 * @param  reg: Register address to write to.
 * @param  value: Value to write to the register.
 * @retval None
 */
void RTC_DS3231_hw_write_register(uint8_t reg, uint8_t value) {
    HAL_StatusTypeDef ret;
    uint8_t data[2] = {reg, value};

    ret = RTC_DS3231_hw_I2C_transmit(data, 2);
    if (ret != HAL_OK) {
        RTC_DS3231_hw_error_handler();
    }
}

/**
 * @brief  Sets the I2C handler for communication with the RTC_DS3231 device.
 * @param  handler: Pointer to the I2C handler.
 * @retval True if the handler was set successfully, false otherwise.
 */
bool_t RTC_DS3231_hw_set_I2C_handler(void *handler) {
    if (handler == NULL) return false;
    i2c_handler = (I2C_HandleTypeDef *)handler;
    return true;
}

/**
 * @brief  Transmits data over I2C with retry mechanism.
 * @param  data: Pointer to the data to transmit.
 * @param  size: Number of bytes to transmit.
 * @retval HAL status of the transmission.
 */
static HAL_StatusTypeDef RTC_DS3231_hw_I2C_transmit(uint8_t *data, uint16_t size) {

    if (data == NULL || size == 0) RTC_DS3231_hw_error_handler();

    HAL_StatusTypeDef ret;
    delay_t timeout;
    DelayInit(&timeout,MAX_DELAY);
    do {
        ret = HAL_I2C_Master_Transmit(i2c_handler, RTC_DS3231_ADDRESS << 1, data, size, MAX_TRANSMISSION_TIMEOUT);
    } while (ret == HAL_BUSY && !DelayRead(&timeout));

    return ret;
}


/**
 * @brief  Receives data over I2C with retry mechanism.
 * @param  data: Pointer to the buffer to receive data.
 * @param  size: Number of bytes to receive.
 * @retval HAL status of the reception.
 */
static HAL_StatusTypeDef RTC_DS3231_hw_I2C_receive(uint8_t *data, uint16_t size) {

    if (data == NULL || size == 0) RTC_DS3231_hw_error_handler();

    HAL_StatusTypeDef ret;
    delay_t timeout;
    DelayInit(&timeout,MAX_DELAY);
    do {
        ret = HAL_I2C_Master_Receive(i2c_handler, RTC_DS3231_ADDRESS << 1, data, size, MAX_TRANSMISSION_TIMEOUT);
    } while (ret == HAL_BUSY && !DelayRead(&timeout));

    return ret;
}

/**
 * @brief  Handles errors.
 * @retval None
 */
static void RTC_DS3231_hw_error_handler(void) {

    while(1) {
        __NOP();
    }
}