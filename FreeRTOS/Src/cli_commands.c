/*
 * CLI_commands.c
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#include "cli_commands.h"
#include "math.h"

#define      PRINT_ABOUT_TASK_STAK_SIZE 128u
#define      PRINT_ABOUT_TASK_PRIORITY  2u
TaskHandle_t Print_About_Task_Handle;
StackType_t  Print_About_Task_Stack[PRINT_ABOUT_TASK_STAK_SIZE];
StaticTask_t Print_About_Task_TCB;
static void Print_About_Task(void* argument);

extern SemaphoreHandle_t CLI_UART_Mutex_Handle;

/*************************************************************************/
void Sprint_Ok_Helper(char* cli_tx_out_buffer, const char* cli_rx_command)
    {
    const char* cmd1 = "\r\nOk:";
    const char* cmd2 = "\r\n";
    strcat(cli_tx_out_buffer, cmd1);
    strcat(cli_tx_out_buffer, cli_rx_command);
    strcat(cli_tx_out_buffer, cmd2);
    }

void Sprint_Error_Helper(char* cli_tx_out_buffer, const char* cli_rx_command)
    {
    const char* cmd1 = "\r\nInvalid Command:";
    const char* cmd2 = "\r\n";
    strcat(cli_tx_out_buffer, cmd1);
    strcat(cli_tx_out_buffer, cli_rx_command);
    strcat(cli_tx_out_buffer, cmd2);
    }

void Sprint_Done_Helper(char* cli_tx_out_buffer, const char* cli_rx_command)
    {
    const char* cmd1 = "\r\nDone\r\n";
    strcat(cli_tx_out_buffer, cmd1);
    }

/*
 *
 */
uint8_t Parse_Integer(const char* param, int32_t* value)
    {

    char str_to_int[11] = "0";
    uint8_t i = 0;
    uint8_t ok_flag = 1;
    int8_t sign = 0;

    if (*param == '-')
	{
	param++;
	sign = -1; //negative
	}
    else
	{
	sign = 1; //positive
	}

    while (*param != '\0' && *param != ' ')
	{

	if (*param < '0' || *param > '9' || i >= 10)
	    {
	    ok_flag = 0; //not number
	    break;
	    }
	str_to_int[i++] = *param++;
	}

    if (ok_flag)
	{
	*value = sign * atoi(str_to_int);
	}

    return ok_flag;
    }

/************CLI Callback Prototypes starts***********************************/
static uint8_t Add_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len);

static uint8_t Sub_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len);

static uint8_t Mul_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len);

static uint8_t Div_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len);

static uint8_t Pow_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len);

static uint8_t Sqrt_Callback(const char* cli_rx_command,
	char* cli_tx_out_buffer, uint16_t max_buffer_len);

/************CLI Callback Prototypes Ends***********************************/

/************CLI Commands Definitions starts***********************************/

CLI_Command_t Add_Defination =
    {
    .CLI_Command = "add", /* The command string to type. */
    .CLI_Command_Description = "\r\n add: addition\r\n", .CLI_Callback =
	    Add_Callback,
    /* The function to run. */
    };

CLI_Command_t Sub_Defination =
    {
    .CLI_Command = "sub", /* The command string to type. */
    .CLI_Command_Description = "\r\n sub: subtraction\r\n", .CLI_Callback =
	    Sub_Callback,
    /* The function to run. */
    };

CLI_Command_t Mul_Defination =
    {
    .CLI_Command = "mul", /* The command string to type. */
    .CLI_Command_Description = "\r\n mul: multiplication\r\n", .CLI_Callback =
	    Mul_Callback,
    /* The function to run. */
    };

CLI_Command_t Div_Defination =
    {
    .CLI_Command = "div", /* The command string to type. */
    .CLI_Command_Description = "\r\n div: division\r\n", .CLI_Callback =
	    Div_Callback,
    /* The function to run. */
    };

CLI_Command_t Pow_Defination =
    {
    .CLI_Command = "pow", /* The command string to type. */
    .CLI_Command_Description = "\r\n pow: raise power\r\n", .CLI_Callback =
	    Pow_Callback,
    /* The function to run. */
    };

CLI_Command_t Sqrt_Defination =
    {
    .CLI_Command = "sqrt", /* The command string to type. */
    .CLI_Command_Description = "\r\n sqrt: square root\r\n", .CLI_Callback =
	    Sqrt_Callback,
    /* The function to run. */
    };

/************CLI Commands Definitions ends***********************************/

/************CLI Callback Implementation starts***********************************/

static uint8_t Add_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 0;
    uint8_t param_count = 0;
    char* param_ptr = NULL;

    int32_t sum = 0;
    int32_t tmp = 0;

    do
	{

	param_number++; //param number, starting from first

	param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

	if (param_ptr != NULL)
	    {
	    param_count++;

	    if (Parse_Integer(param_ptr, &tmp))
		{
		sum += tmp;
		}
	    else
		{
		is_command_valid = 0;
		}
	    }

	}
    while (param_ptr != NULL);

    /*at least one parameter is expected*/
    if (!param_count)
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{

	sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) sum);

	}
    else
	{
	Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}
    return 0;
    }

static uint8_t Sub_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 0;
    uint8_t param_count = 0;
    char* param_ptr = NULL;

    int32_t rslt = 0;
    int32_t tmp = 0;

    do
	{

	param_number++; //param number, starting from first

	param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

	if (param_ptr != NULL)
	    {
	    param_count++;

	    if (Parse_Integer(param_ptr, &tmp))
		{
		/*if first param*/
		if (param_count == 1)
		    {
		    rslt = tmp;
		    }
		else
		    {
		    rslt -= tmp;
		    }
		}
	    else
		{
		is_command_valid = 0;
		}
	    }

	}
    while (param_ptr != NULL);

    /*at least one parameter is expected*/
    if (!param_count)
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{

	sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	}
    else
	{
	Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;

    }

static uint8_t Mul_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 0;
    uint8_t param_count = 0;
    char* param_ptr = NULL;

    int32_t rslt = 1;
    int32_t tmp = 0;

    do
	{

	param_number++; //param number, starting from first

	param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

	if (param_ptr != NULL)
	    {

	    param_count++;

	    if (Parse_Integer(param_ptr, &tmp))
		{
		rslt *= tmp;
		}
	    else
		{
		is_command_valid = 0;
		}
	    }

	}
    while (param_ptr != NULL);

    /*at least one parameter is expected*/
    if (!param_count)
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{

	sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	}
    else
	{
	Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;

    }

static uint8_t Div_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 0;
    uint8_t param_count = 0;
    char* param_ptr = NULL;

    int32_t rslt = 1;
    int32_t tmp = 0;

    do
	{

	param_number++; //param number, starting from first

	param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

	if (param_ptr != NULL)
	    {
	    param_count++;

	    if (Parse_Integer(param_ptr, &tmp))
		{
		/*if first param*/
		if (param_count == 1)
		    {
		    rslt = tmp;
		    }
		else
		    {
		    rslt /= tmp;
		    }
		}
	    else
		{
		is_command_valid = 0;
		}
	    }

	}
    while (param_ptr != NULL);

    /*only two parameters are expected*/
    if (param_count != 2)
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{

	sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	}
    else
	{
	Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;

    }

static uint8_t Pow_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t max_buffer_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 0;
    uint8_t param_count = 0;
    char* param_ptr = NULL;

    int32_t rslt = 1;
    int32_t tmp = 0;

    do
	{

	param_number++; //param number, starting from first

	param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

	if (param_ptr != NULL)
	    {
	    param_count++;

	    if (Parse_Integer(param_ptr, &tmp))
		{
		/*if first param*/
		if (param_count == 1)
		    {
		    rslt = tmp;
		    }
		else
		    {
		    rslt = pow(rslt, tmp);
		    }
		}
	    else
		{
		is_command_valid = 0;
		}
	    }

	}
    while (param_ptr != NULL);

    /*only twh parameters are expected*/
    if (param_count != 2)
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{

	sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	}
    else
	{
	Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;

    }

static uint8_t Sqrt_Callback(const char* cli_rx_command,
	char* cli_tx_out_buffer, uint16_t max_buffer_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 0;
    uint8_t param_count = 0;
    char* param_ptr = NULL;

    int32_t rslt = 1;
    int32_t tmp = 0;

    do
	{

	param_number++; //param number, starting from first

	param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

	if (param_ptr != NULL)
	    {
	    param_count++;
	    if (Parse_Integer(param_ptr, &tmp))
		{
		rslt = sqrt(tmp);
		}
	    else
		{
		is_command_valid = 0;
		}
	    }

	}
    while (param_ptr != NULL);

    /*only one parameter is expected*/
    if (param_count != 1)
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{

	sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	}
    else
	{
	Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;
    }

/************CLI callback Implementation starts***********************************/

/*
 *
 *
 *
 */
void CLI_Add_All_Commands()
    {

    CLI_Add_Cammand(&Add_Defination);
    CLI_Add_Cammand(&Sub_Defination);
    CLI_Add_Cammand(&Mul_Defination);
    CLI_Add_Cammand(&Div_Defination);
    CLI_Add_Cammand(&Pow_Defination);
    CLI_Add_Cammand(&Sqrt_Defination);

    Print_About_Task_Handle = xTaskCreateStatic(Print_About_Task,
	    "Print_About_Task",
	    PRINT_ABOUT_TASK_STAK_SIZE,
	    NULL,
	    PRINT_ABOUT_TASK_PRIORITY, Print_About_Task_Stack,
	    &Print_About_Task_TCB);

    }

void Print_About_Task(void* argument)
    {

    /*** gaurd uart ***/
    xSemaphoreTake(CLI_UART_Mutex_Handle, portMAX_DELAY);

    CLI_UART_Send_String("\r\nSTM32 CLI over uart demo.\r\nCalculator app\r\n");

    /*** release uart ***/
    xSemaphoreGive(CLI_UART_Mutex_Handle);

    vTaskSuspend(Print_About_Task_Handle);

    while (1)
	{

	}
    }
