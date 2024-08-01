/*
 * API_uart.h
 *
 *  Created on: Jul 25, 2024
 *      Author: José Luis Krüger
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/**
 * @brief  Initializes the UART peripheral.
 * @retval bool_t: Returns true if initialization is successful, otherwise false.
 */
bool_t uartInit(void);

/**
 * @brief  Sends a string over UART.
 * @param  pstring: Pointer to the string to be sent.
 * @retval None
 */
void uartSendString(uint8_t *pstring);

/**
 * @brief  Sends a string of specified size over UART.
 * @param  pstring: Pointer to the string to be sent.
 * @param  size: Size of the string to be sent.
 * @retval None
 */
void uartSendStringSize(uint8_t *pstring, uint16_t size);

/**
 * @brief  Receives a string of specified size over UART.
 * @param  pstring: Pointer to the buffer where the received string will be stored.
 * @param  size: Size of the buffer to store the received string.
 * @retval None
 */
void uartReceiveStringSize(uint8_t *pstring, uint16_t size);


#endif /* API_INC_API_UART_H_ */
