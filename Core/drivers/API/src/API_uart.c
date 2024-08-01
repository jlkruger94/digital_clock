/*
 * API_uart.c
 *
 *  Created on: Jul 25, 2024
 *      Author: José Luis Krüger
 */

#include "API_uart.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
       set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* UART handler declaration */
static UART_HandleTypeDef UartHandle;
static const uint16_t MAX_BUFFER_LENGTH = 6500;
static const uint32_t BAUD_RATE = 9600;

/* Private function prototypes */
static void UartErrorHandler(void);
static void GetParityString(char *, uint32_t);
static void GetModeString(char *, uint32_t);

/**
 * @brief  Initializes the UART peripheral.
 * @retval bool_t: Returns true if initialization is successful, otherwise false.
 */
bool_t uartInit(void)
{
    /* Configure the UART peripheral */
    /* UART configured as follows:
        - Word Length = 8 Bits
        - Stop Bit = One Stop bit
        - Parity = ODD parity
        - BaudRate = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals)
    */
    UartHandle.Instance        = USARTx;

    UartHandle.Init.BaudRate   = BAUD_RATE;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_ODD;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
    	UartErrorHandler();
        return false;
    }

    char parity[20] = {0};
    char mode[20] = {0};

    GetParityString(parity, UartHandle.Init.Parity);
    GetModeString(mode, UartHandle.Init.Mode);

    uint8_t buffer[256] = {0};
    sprintf((char*)buffer, "\r\nParameters configured: \n\r-Baudrate: %u\n\r-WordLength: %u\n\r-StopBits: %u\n\r-Parity: %s\n\r-Mode: %s\n\r-OverSampling: %u",
            (unsigned int)UartHandle.Init.BaudRate,
            (unsigned int)UartHandle.Init.WordLength,
            (unsigned int)UartHandle.Init.StopBits,
            parity,
            mode,
            (unsigned int)UartHandle.Init.OverSampling);

    uartSendString(buffer);
    return true;
}

/**
 * @brief  Sends a string over UART.
 * @param  pstring: Pointer to the string to be sent.
 * @retval None
 */
void uartSendString(uint8_t *pstring)
{
    // Parameters assertion
    if (pstring == NULL) return;
    uint16_t buffer_length = (uint16_t)strlen((char *)pstring);
    if (buffer_length > MAX_BUFFER_LENGTH) return;

    HAL_UART_Transmit(&UartHandle, pstring, buffer_length, HAL_MAX_DELAY);
}

/**
 * @brief  Sends a string of specified size over UART.
 * @param  pstring: Pointer to the string to be sent.
 * @param  size: Size of the string to be sent.
 * @retval None
 */
void uartSendStringSize(uint8_t *pstring, uint16_t size)
{
    // Parameters assertion
    if (pstring == NULL || size > MAX_BUFFER_LENGTH) return;

    HAL_UART_Transmit(&UartHandle, pstring, size, HAL_MAX_DELAY);
}

/**
 * @brief  Receives a string of specified size over UART.
 * @param  pstring: Pointer to the buffer where the received string will be stored.
 * @param  size: Size of the buffer to store the received string.
 * @retval None
 */
void uartReceiveStringSize(uint8_t *pstring, uint16_t size)
{
    // Parameters assertion
    if (pstring == NULL || size > MAX_BUFFER_LENGTH) return;

    uint16_t i = 0;
    uint8_t ch;
    HAL_StatusTypeDef status;

    while (i < size - 1) {
        status = HAL_UART_Receive(&UartHandle, &ch, 1, HAL_MAX_DELAY);
        if (status == HAL_OK) {
            if (ch == '\n' || ch == '\r') {
                break;
            }
            pstring[i++] = ch;
        } else {
        	UartErrorHandler();
            break;
        }
    }
    pstring[i] = '\0';
}

/**
 * @brief Handles UART errors by turning on an error LED.
 * @retval None
 */
static void UartErrorHandler(void)
{
    BSP_LED_On(LED3);
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  ch: Character to be transmitted.
 * @retval Character transmitted.
 */
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/**
 * @brief  Converts UART parity setting to a string representation.
 * @param  buffer: Pointer to the buffer where the string will be stored.
 * @param  parity: UART parity setting.
 * @retval None
 */
static void GetParityString(char *buffer, uint32_t parity)
{
    if (buffer == NULL) {
        UartErrorHandler();
        return;
    }
    switch (parity) {
        case UART_PARITY_NONE:
            sprintf(buffer, "%s", "NONE");
            break;
        case UART_PARITY_EVEN:
            sprintf(buffer, "%s", "EVEN");
            break;
        case UART_PARITY_ODD:
            sprintf(buffer, "%s", "ODD");
            break;
        default:
            UartErrorHandler();
            break;
    }
}

/**
 * @brief  Converts UART mode setting to a string representation.
 * @param  buffer: Pointer to the buffer where the string will be stored.
 * @param  mode: UART mode setting.
 * @retval None
 */
static void GetModeString(char *buffer, uint32_t mode)
{
    if (buffer == NULL) {
        UartErrorHandler();
        return;
    }
    switch (mode) {
        case UART_MODE_RX:
            sprintf(buffer, "%s", "RX");
            break;
        case UART_MODE_TX:
            sprintf(buffer, "%s", "TX");
            break;
        case UART_MODE_TX_RX:
            sprintf(buffer, "%s", "TX_RX");
            break;
        default:
            UartErrorHandler();
            break;
    }
}
