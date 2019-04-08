/*
 * cli_uart_interface.c
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

#include "cli_uart_interface.h"

#define UART_RING_BUFFER_SIZE   128
#define OUTPUT_BUFFER_SIZE      128
#define INPUT_BUFFER_SIZE       128

static char   UART_DMA_RX_Buffer[UART_RING_BUFFER_SIZE];
static Ring_Buffer_t UART_Ring_Buffer_Handle;

/*data is written to buffer via uart DMA in background*/
/* need to update Write_Index manually */
#define     UPDATE_RING_BUFFER() (UART_Ring_Buffer_Handle.Write_Index = ( UART_RING_BUFFER_SIZE - (CLI_UART->hdmarx->Instance->NDTR)))


static char CLI_Output_Buffer[OUTPUT_BUFFER_SIZE]; //cli output
static char CLI_CMD_Buffer[INPUT_BUFFER_SIZE];

extern UART_HandleTypeDef huart2;
UART_HandleTypeDef* CLI_UART = &huart2;

void CLI_UART_Init()
    {
    CLI_Add_Help_Cammand();
    Ring_Buffer_Init(&UART_Ring_Buffer_Handle,UART_DMA_RX_Buffer, UART_RING_BUFFER_SIZE);
    HAL_UART_Receive_DMA(CLI_UART, (uint8_t*) UART_DMA_RX_Buffer, UART_RING_BUFFER_SIZE);
    CLI_UART_Send_String("\n->");
    }

void CLI_UART_Send_Char(char data)
    {
    CLI_UART->Instance->DR = (data);
    while (__HAL_UART_GET_FLAG(CLI_UART,UART_FLAG_TC) == 0);
    }

void CLI_UART_Send_String(char* data)
    {
    uint16_t count = 0;
    while (*data)
	{
	CLI_UART_Send_Char(*data++);
	count++;
	if (count == OUTPUT_BUFFER_SIZE)
	    {
	    break;
	    }
	}
    }

void CLI_UART_Loop()
    {

    static uint8_t rx_char_count;
    char rx_char;
    uint8_t call_again;

    /*data is written to buffer via uart DMA in background*/
    /* need to update Write_Index manually */
    UPDATE_RING_BUFFER();

    while (Ring_Buffer_Get_Count(&UART_Ring_Buffer_Handle))
	{

	Ring_Buffer_Get_Char(&UART_Ring_Buffer_Handle, &rx_char);

	if (rx_char == '\r')
	    {

	    rx_char_count = 0; //reset CLI_CMD_Buffer index
	    Ring_Buffer_Get_Char(&UART_Ring_Buffer_Handle, &rx_char); //remove \n, if there is
	    Ring_Buffer_Flush(&UART_Ring_Buffer_Handle); //reset ring buffer

	    // process cammand
	    do
		{

		memset(CLI_Output_Buffer, 0x00, OUTPUT_BUFFER_SIZE); //reset output buffer

		call_again = CLI_Process_Cammand(CLI_CMD_Buffer,
			CLI_Output_Buffer, OUTPUT_BUFFER_SIZE);

		if (CLI_Output_Buffer[0] != '\0')
		    {
		    //send output to console
		    CLI_UART_Send_String(CLI_Output_Buffer);
		    }
		}
	    while (call_again);

	    CLI_UART_Send_String("\n->");
	    }
	else //else update command buffer
	    {

	    if (!rx_char_count)
		{
		memset(CLI_CMD_Buffer, 0x00, INPUT_BUFFER_SIZE); //reset cmd buffer
		}

	    if ((rx_char == '\b') || (rx_char == 0x7F)) // backspace or delete
		{
		/* Backspace was pressed.  Erase the last character in the
		 string - if any. */
		if (rx_char_count > 0)
		    {
		    rx_char_count--;
		    CLI_CMD_Buffer[rx_char_count] = '\0';
		    }
		}
	    else
		{
		// accumulate characters in input buffer
		if ((rx_char >= ' ') && (rx_char <= '~'))
		    {
		    if (rx_char_count < INPUT_BUFFER_SIZE)
			{
			CLI_CMD_Buffer[rx_char_count] = rx_char;
			rx_char_count++;
			}
		    }
		}
	    }
	}
    }
