/*
 * uart_cli.c
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

#include "cli_uart.h"

#define OUTPUT_BUFFER_SIZE 500
#define INPUT_BUFFER_SIZE  100

#define MAX_COMMANDS       50

static char CLI_Output_Buffer[OUTPUT_BUFFER_SIZE]; //cli output
static char CLI_Input_Buffer[INPUT_BUFFER_SIZE];
static char CLI_CMD_Buffer[INPUT_BUFFER_SIZE];

extern UART_HandleTypeDef huart2;
UART_HandleTypeDef* CLI_UART = &huart2;

static uint16_t Command_Count = 0;
CLI_Command_t* Cammand_List[MAX_COMMANDS];

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
	if(count == OUTPUT_BUFFER_SIZE)
	    {
	    break;
	    }
	}
    }


uint8_t Help_Callback(char* cli_rx_command, char* cli_tx_out_buffer, uint16_t cmd_len)
    {

    static uint16_t count = 0;

    CLI_Command_t* command_list_ptr = command_list_ptr = Cammand_List[count];;

    strncpy(cli_tx_out_buffer,
	    command_list_ptr->CLI_Command_Description,
	    command_list_ptr->CLI_Description_Length+1);// add null char at end

    count++;
    if (count < Command_Count)
	{
	return 1; //call again to generate next output
	}

    count = 0;

    return 0; // operation complete do not call again

    }

CLI_Command_t Help_Defination =
    {
    .CLI_Command = "help",
    .CLI_Command_Description = "\r\n help: Shows the list of available commands\r\n",
    .CLI_Callback = Help_Callback // this function will be call repeatedly until it returns 0
	                          // require only if generated output in callback is larger than output buffer
    };                            // to split generated output


void CLI_UART_Init()
    {

    CLI_Add_Cammand(&Help_Defination); // add help command
    Ring_Buffer_Init(CLI_UART);
    CLI_UART_Send_String("\n->");

    }


uint8_t CLI_Add_Cammand(CLI_Command_t* command_def)
    {

    if (Command_Count < MAX_COMMANDS)
	{
	command_def->CLI_Command_Length = strlen(command_def->CLI_Command);
	command_def->CLI_Description_Length = strlen(command_def->CLI_Command_Description);
	Cammand_List[Command_Count] = command_def;
	Command_Count++;
	return 1; // command added successful
	}

    return 0;

    }


void CLI_UART_Loop()
    {

    static uint8_t rx_char_count;
    uint8_t rx_char;
    uint8_t is_command_valid = 0;

    CLI_Command_t* command_list_ptr = NULL;

    if (Ring_Buffer_Get_Count() > 0)
	{

	Ring_Buffer_Get_Char(&rx_char);

	if (rx_char == '\n') // end of command
	    {

	    if (rx_char_count != 0) // new commnad
		{
		strncpy(CLI_CMD_Buffer, CLI_Input_Buffer, INPUT_BUFFER_SIZE); // copy command
		memset(CLI_Input_Buffer, 0x00, INPUT_BUFFER_SIZE); //reset input buffer
		rx_char_count = 0;
		}

	    //scan for command
	    for (uint16_t i = 0; i < Command_Count; i++)
		{

		command_list_ptr = Cammand_List[i];

		if (strncmp(CLI_CMD_Buffer, command_list_ptr->CLI_Command,
			command_list_ptr->CLI_Command_Length+1) == 0)
		    {

		    is_command_valid = 1;

		    if (command_list_ptr->CLI_Callback != NULL)
			{
			uint8_t call_again = 1;
			// repeatedly call this function until it returns 0
			// require only if generated output in callback is larger than output buffer
			while (call_again)
			    {

		            memset(CLI_Output_Buffer, 0x00, OUTPUT_BUFFER_SIZE); //reset output buffer

		            //callback function
			    call_again = command_list_ptr->CLI_Callback(
				    CLI_CMD_Buffer,
				    CLI_Output_Buffer,
				    command_list_ptr->CLI_Command_Length);

			    //send output to console
			    CLI_UART_Send_String(CLI_Output_Buffer);

			    }
			}
		    break; // command found break the loop

		    }

		}

	    if (!is_command_valid)
		{

	       // memset(CLI_Output_Buffer, 0x00, OUTPUT_BUFFER_SIZE); //reset output buffer

		strncpy(CLI_Output_Buffer, CLI_CMD_Buffer, INPUT_BUFFER_SIZE);
		strncat(CLI_Output_Buffer,
			":Command not recognized.  Enter 'help' to view a list of available commands.\r\n\r\n",
			OUTPUT_BUFFER_SIZE);
		//send output to console
		CLI_UART_Send_String(CLI_Output_Buffer);
		}

	    CLI_UART_Send_String("\r\n->");

	    }
	else
	    {

	    if (rx_char == '\r')
		{
		/* Ignore the character. */
		}
	    else if ((rx_char == '\b') || (rx_char == 0x7F)) // backspace or delete
		{
		/* Backspace was pressed.  Erase the last character in the
		 string - if any. */
		if (rx_char_count > 0)
		    {
		    rx_char_count--;
		    CLI_Input_Buffer[rx_char_count] = '\0';
		    }
		}
	    else
		{
		// accumulate characters in input buffer
		if ((rx_char >= ' ') && (rx_char <= '~'))
		    {
		    if (rx_char_count < INPUT_BUFFER_SIZE)
			{
			CLI_Input_Buffer[rx_char_count] = rx_char;
			rx_char_count++;
			}
		    }
		}

	    }
	}
    }



char* CLI_UART_Get_Parameter(char *pcCommandString, uint16_t uxWantedParameter,
	uint16_t *pxParameterStringLength)
    {

    uint16_t uxParametersFound = 0;
    char *pcReturn = NULL;

    *pxParameterStringLength = 0;

    while (uxParametersFound < uxWantedParameter)
	{
	/* Index the character pointer past the current word.  If this is the start
	 of the command string then the first word is the command itself. */
	while (((*pcCommandString) != 0x00) && ((*pcCommandString) != ' '))
	    {
	    pcCommandString++;
	    }

	/* Find the start of the next string. */
	while (((*pcCommandString) != 0x00) && ((*pcCommandString) == ' '))
	    {
	    pcCommandString++;
	    }

	/* Was a string found? */
	if (*pcCommandString != 0x00)
	    {
	    /* Is this the start of the required parameter? */
	    uxParametersFound++;

	    if (uxParametersFound == uxWantedParameter)
		{
		/* How long is the parameter? */
		pcReturn = pcCommandString;
		while (((*pcCommandString) != 0x00)
			&& ((*pcCommandString) != ' '))
		    {
		    (*pxParameterStringLength)++;
		    pcCommandString++;
		    }

		if (*pxParameterStringLength == 0)
		    {
		    pcReturn = NULL;
		    }

		break;
		}
	    }
	else
	    {
	    break;
	    }
	}

    return pcReturn;
    }
