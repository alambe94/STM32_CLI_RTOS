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

/**
 * @file  cli.c
 * @brief using static array instead of linked list
 * @author medprime (www.medprimetech.com)
 * @version 0.0.0
 **/

#include "cli.h"
#include "string.h"
#include "stdio.h"

#define MAX_COMMANDS 100
#define MAX_ARGS_IN_CMD 10

static uint16_t Command_Count = 0;

/**
 * @brief list of all registered commands
 **/
static CLI_Command_t *Cammand_List[MAX_COMMANDS];

/**
 * @brief help callback prototype
 **/
static uint8_t Help_Callback(uint8_t argc, const char *argv[],
	char *cli_out_buffer, uint16_t cli_out_max);

/**
 * @brief definition of help command
 **/
static CLI_Command_t Help_Defination =
    {
    /* command string to type */
    .CLI_Command = "help",

    /* command help string */
    .CLI_Command_Description =
	    "\r\n help: Shows the list of available commands\r\n",

    /* function to run. */
    .CLI_Callback = Help_Callback
    };

/**
 * @brief add help command to command list
 **/
void CLI_Init()
    {
    CLI_Add_Cammand(&Help_Defination);
    }

/**
 * @brief add given command to list of registered commands
 * @param command_def command to be registered
 * @see CLI_Init for example
 * @note adjust MAX_COMMANDS accordingly
 **/
uint8_t CLI_Add_Cammand(CLI_Command_t *command_def)
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

/**
 * @brief process input string, compare input string with registered commands, if match
 *        found call the corresponding callback function
 * @param cli_in_buffer input string to be processed
 * @param cli_out_buffer generated output will be written to this buffer
 * @param cli_out_max maximum number of char that can be written to cli_out_buffer
 * @see CLI_Init for example
 * @note adjust MAX_COMMANDS accordingly
 **/
uint8_t CLI_Process_Cammand(const char *cli_in_buffer, char *cli_out_buffer,
	uint16_t cli_out_max)
    {

    uint8_t is_command_valid = 0;
    uint8_t xreturn = 0;
    uint8_t argc = 0;
    const char *argv[MAX_ARGS_IN_CMD] =
	{
	0
	};
    CLI_Command_t *command_list_ptr = NULL;

    /* Search for the command string in the list of registered commands. */
    for (uint16_t i = 0; i < Command_Count; i++)
	{

	command_list_ptr = Cammand_List[i];
	uint16_t cmd_len = command_list_ptr->CLI_Command_Length;

	/* To ensure the string lengths match exactly, so as not to pick up
	 a sub-string of a longer command, check the byte after the expected
	 end of the string is either the end of the string or a space before
	 a parameter. */
	if ((cli_in_buffer[cmd_len] == ' ') || (cli_in_buffer[cmd_len] == 0x00))
	    {

	    if (strncmp(cli_in_buffer, command_list_ptr->CLI_Command, cmd_len)
		    == 0)
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
	    CLI_Parse_Arguments(cli_in_buffer, &argc, argv);
	    xreturn = command_list_ptr->CLI_Callback(argc, argv, cli_out_buffer,
		    cli_out_max);
	    }
	}
    else
	{
	snprintf(cli_out_buffer, cli_out_max, "\r\nError->%s CMD not found\r\n",
		cli_in_buffer);
	}

    return xreturn;
    }

void CLI_Parse_Arguments(const char *cli_in_buffer, uint8_t *argc,
	const char *argv[])
    {
    uint8_t argc_temp = 0;
    /* arg 0 is input cmd*/
    argv[argc_temp++] = cli_in_buffer;

    while (argc_temp < MAX_ARGS_IN_CMD)
	{
	while (((*cli_in_buffer) != 0x00) && ((*cli_in_buffer) != ' '))
	    {
	    cli_in_buffer++;
	    }

	while ((*cli_in_buffer) == ' ')
	    {
	    cli_in_buffer++;
	    }

	if (*cli_in_buffer != 0x00)
	    {
	    argv[argc_temp++] = cli_in_buffer;
	    }
	else
	    {
	    break;
	    }
	}
    *argc = argc_temp;
    }

uint8_t CLI_Get_Argument_Length(const char *arg)
    {
    uint8_t temp = 0;
    while (((*arg) != 0x00) && ((*arg) != ' '))
	{
	arg++;
	temp++;
	}
    return temp;
    }

/**
 * @brief callback function will be call repeatedly until it returns 0
 *  require only if generated output in callback is larger than output buffer
 *  to split generated output
 **/
static uint8_t Help_Callback(uint8_t argc, const char *argv[],
	char *cli_out_buffer, uint16_t cli_out_max)
    {

    static uint16_t count = 0;

    CLI_Command_t *command_list_ptr = Cammand_List[count];

    strncpy(cli_out_buffer, command_list_ptr->CLI_Command_Description,
	    cli_out_max);

    count++;
    if (count < Command_Count)
	{
	return 1; //call again to generate next output
	}

    count = 0;

    return 0; // operation complete do not call again
    }
