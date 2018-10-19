/*
 * uart_parser_freertos.h
 *
 *  Created on: 15-Oct-2018
 *      Author: medprime
 */

#ifndef UART_CLI_INTERFACE_H_
#define UART_CLI_INTERFACE_H_


#include "stm32f4xx_hal.h"
#include "FreeRTOS_CLI.h"
#include "string.h"
#include "ring_buffer.h"

void xSerialPutChar(UART_HandleTypeDef *huart, uint8_t pData);

void vSerialPutString(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size);

void UARTCommandConsoleLoop(void);





#endif /* UART_CLI_INTERFACE_H_ */
