/*
 * freeRTOS_cli.c
 *
 *  Created on: 22-Mar-2019
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

#include "freeRTOS_cli.h"

#define MAX_COMMANDS       50

static uint16_t Command_Count = 0;

static CLI_Command_t* Cammand_List[MAX_COMMANDS];

static const char* const CMD_Not_Recognized_Sring =
	":Command not recognized.  Enter 'help' to view a list of available commands.\r\n\r\n";


uint8_t CLI_Add_Cammand(CLI_Command_t* command_def)
    {

    if (Command_Count < MAX_COMMANDS)
	{
	command_def->CLI_Command_Length = strlen(command_def->CLI_Command);
	Cammand_List[Command_Count] = command_def;
	Command_Count++;
	return 1; // command added successful
	}

    return 0;

    }

uint8_t CLI_Process_Cammand(char* cli_in_buffer, char* cli_tx_out_buffer)
    {

    uint8_t is_command_valid = 0;
    uint8_t xreturn = 0;
    CLI_Command_t* command_list_ptr = NULL;


    /* Search for the command string in the list of registered commands. */
    for (uint16_t i = 0; i < Command_Count; i++)
	{
	command_list_ptr = Cammand_List[i];
	uint16_t cmd_len = command_list_ptr->CLI_Command_Length;

	/* To ensure the string lengths match exactly, so as not to pick up
	 a sub-string of a longer command, check the byte after the expected
	 end of the string is either the end of the string or a space before
	 a parameter. */
	if ((cli_in_buffer[cmd_len] == ' ')
		|| (cli_in_buffer[cmd_len] == 0x00))
	    {

	    if (strncmp(cli_in_buffer, command_list_ptr->CLI_Command,
		    cmd_len) == 0)
		{
		is_command_valid = 1;
		break; // command found break the loop
		}
	    }
	}

    if (is_command_valid)
	{
	if (command_list_ptr->CLI_Callback != NULL)
	    {
		xreturn = command_list_ptr->CLI_Callback(
			cli_in_buffer, cli_tx_out_buffer);
	    }
	}
    else
	{
	strncpy(cli_tx_out_buffer, cli_in_buffer, strlen(cli_in_buffer));
	strncat(cli_tx_out_buffer, CMD_Not_Recognized_Sring,strlen(CMD_Not_Recognized_Sring));
	}

    return xreturn;

    }


char* CLI_Get_Parameter(char *cli_in_buffer, uint16_t param_number,
	uint16_t *paran_number_len)
    {

    uint16_t uxParametersFound = 0;
    char *pcReturn = NULL;

    *paran_number_len = 0;

    while (uxParametersFound < param_number)
	{
	/* Index the character pointer past the current word.  If this is the start
	 of the command string then the first word is the command itself. */
	while (((*cli_in_buffer) != 0x00) && ((*cli_in_buffer) != ' '))
	    {
	    cli_in_buffer++;
	    }

	/* Find the start of the next string. */
	while (((*cli_in_buffer) != 0x00) && ((*cli_in_buffer) == ' '))
	    {
	    cli_in_buffer++;
	    }

	/* Was a string found? */
	if (*cli_in_buffer != 0x00)
	    {
	    /* Is this the start of the required parameter? */
	    uxParametersFound++;

	    if (uxParametersFound == param_number)
		{
		/* How long is the parameter? */
		pcReturn = cli_in_buffer;
		while (((*cli_in_buffer) != 0x00)
			&& ((*cli_in_buffer) != ' '))
		    {
		    (*paran_number_len)++;
		    cli_in_buffer++;
		    }

		if (*paran_number_len == 0)
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



uint8_t Help_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    static uint16_t count = 0;

    CLI_Command_t* command_list_ptr = Cammand_List[count];

    strncpy(cli_tx_out_buffer, command_list_ptr->CLI_Command_Description,
	    strlen(command_list_ptr->CLI_Command_Description));

    count++;
    if (count < Command_Count)
	{
	return 1; //call again to generate next output
	}

    count = 0;

    return 0; // operation complete do not call again

    }


static CLI_Command_t Help_Defination =
    {
	    .CLI_Command = "help",
	    .CLI_Command_Description =
		    "\r\n help: Shows the list of available commands\r\n",
	    .CLI_Callback = Help_Callback
    // this function will be call repeatedly until it returns 0
    // require only if generated output in callback is larger than output buffer
    // to split generated output
	};

void CLI_Add_Help_Cammand()
    {
    CLI_Add_Cammand(&Help_Defination);
    }
