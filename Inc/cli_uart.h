/*
 * uart_cli.h
 *
 *  Created on: 06-Mar-2019
 *      Author: medprime
 */

/*
 * FreeRTOS+CLI V1.0.4
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef UART_CLI_H_
#define UART_CLI_H_

#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "ring_buffer.h"
#include "string.h"


typedef struct
    {
	const char* CLI_Command;
	const char* CLI_Command_Description;
	uint16_t CLI_Command_Length;
	uint16_t CLI_Description_Length;
	uint8_t (*CLI_Callback)(char* cli_rx_command, char* cli_tx_out_buffer, uint16_t cmd_len);
    } CLI_Command_t;


void CLI_UART_Init();
void CLI_UART_Loop();
void CLI_UART_Send_String(char* data);
uint8_t CLI_Add_Cammand(CLI_Command_t* command_def);
char* CLI_UART_Get_Parameter(char *pcCommandString,
	uint16_t uxWantedParameter,
	uint16_t *pxParameterStringLength);




#endif /* UART_CLI_H_ */
