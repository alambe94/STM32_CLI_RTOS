/*
 * CLI_commands.c
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#include "cli_commands.h"
#include "printf.h"
#include "math.h"

#define      PRINT_ABOUT_TASK_STAK_SIZE 128u
#define      PRINT_ABOUT_TASK_PRIORITY  2u
TaskHandle_t Print_About_Task_Handle;
StackType_t  Print_About_Task_Stack[PRINT_ABOUT_TASK_STAK_SIZE];
StaticTask_t Print_About_Task_TCB;
static void  Print_About_Task(void* argument);

extern SemaphoreHandle_t CLI_UART_Mutex_Handle;


#define configINCLUDE_QUERY_HEAP_COMMAND 1

/*************************************************************************/
void Sprint_Ok_Helper(char* cli_tx_out_buffer, const char* cli_rx_command) {
	const char* cmd1 = "\r\nOk:";
	const char* cmd2 = "\r\n";
	strcat(cli_tx_out_buffer, cmd1);
	strcat(cli_tx_out_buffer, cli_rx_command);
	strcat(cli_tx_out_buffer, cmd2);
}

void Sprint_Error_Helper(char* cli_tx_out_buffer, const char* cli_rx_command) {
	const char* cmd1 = "\r\nInvalid Command:";
	const char* cmd2 = "\r\n";
	strcat(cli_tx_out_buffer, cmd1);
	strcat(cli_tx_out_buffer, cli_rx_command);
	strcat(cli_tx_out_buffer, cmd2);
}

void Sprint_Done_Helper(char* cli_tx_out_buffer, const char* cli_rx_command) {
	const char* cmd1 = "\r\nDone\r\n";
	strcat(cli_tx_out_buffer, cmd1);
}

/*
 *
 */
uint8_t Parse_Integer(const char* param, int32_t* value) {

	char str_to_int[11] = "0";
	uint8_t i = 0;
	uint8_t ok_flag = 1;
	int8_t sign = 0;

	if (*param == '-') {
		param++;
		sign = -1; //negative
	} else {
		sign = 1; //positive
	}

	while (*param != '\0' && *param != ' ') {

		if (*param < '0' || *param > '9' || i >= 10) {
			ok_flag = 0; //not number
			break;
		}
		str_to_int[i++] = *param++;
	}

	if (ok_flag) {
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

/*
 * Implements the task-stats command.
 */
static uint8_t prvTaskStatsCommand(const char *pcCommandString,
		                              char *pcWriteBuffer,
		                              uint16_t xWriteBufferLen);

/*
 * Implements the run-time-stats command.
 */
#if( configGENERATE_RUN_TIME_STATS == 1 )
static uint8_t prvRunTimeStatsCommand(const char *pcCommandString,
		                              char *pcWriteBuffer,
		                              uint16_t xWriteBufferLen);
#endif /* configGENERATE_RUN_TIME_STATS */

/*
 * Implements the "query heap" command.
 */
#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
		static uint8_t prvQueryHeapCommand(const char *pcCommandString,
				                           char *pcWriteBuffer,
				                           uint16_t xWriteBufferLen);
#endif

/*
 * Implements the "trace start" and "trace stop" commands;
 */
#if( configUSE_TRACE_FACILITY == 1 )
static uint8_t prvStartStopTraceCommand(const char *pcCommandString,
		                                char *pcWriteBuffer,
		                                uint16_t xWriteBufferLen);
#endif
/************CLI Callback Prototypes Ends***********************************/

/************CLI Commands Definitions starts***********************************/

CLI_Command_t Add_Defination = {
		.CLI_Command = "add", /* The command string to type. */
        .CLI_Command_Description = "\r\n add: addition\r\n",
		.CLI_Callback = Add_Callback,
/* The function to run. */
};

CLI_Command_t Sub_Defination = {
		.CLI_Command = "sub", /* The command string to type. */
        .CLI_Command_Description = "\r\n sub: subtraction\r\n",
		.CLI_Callback = Sub_Callback,
/* The function to run. */
};

CLI_Command_t Mul_Defination = {
		.CLI_Command = "mul", /* The command string to type. */
        .CLI_Command_Description = "\r\n mul: multiplication\r\n",
		.CLI_Callback = Mul_Callback,
/* The function to run. */
};

CLI_Command_t Div_Defination = {
		.CLI_Command = "div", /* The command string to type. */
        .CLI_Command_Description = "\r\n div: division\r\n",
		.CLI_Callback = Div_Callback,
/* The function to run. */
};

CLI_Command_t Pow_Defination = {
		.CLI_Command = "pow", /* The command string to type. */
        .CLI_Command_Description = "\r\n pow: raise power\r\n",
		.CLI_Callback = Pow_Callback,
/* The function to run. */
};

CLI_Command_t Sqrt_Defination = {
		.CLI_Command = "sqrt", /* The command string to type. */
        .CLI_Command_Description = "\r\n sqrt: square root\r\n",
		.CLI_Callback = Sqrt_Callback,
/* The function to run. */
};

/* Structure that defines the "task-stats" command line command.  This generates
 a table that gives information on each task in the system. */
CLI_Command_t xTaskStats =
		{
				.CLI_Command = "task-stats", /* The command string to type. */
				.CLI_Command_Description =
						"\r\n task-stats: Displays a table showing the state of each FreeRTOS task\r\n",
				.CLI_Callback = prvTaskStatsCommand, /* The function to run. */
		};

#if( configGENERATE_RUN_TIME_STATS == 1 )
/* Structure that defines the "run-time-stats" command line command.   This
 generates a table that shows how much run time each task has */
CLI_Command_t xRunTimeStats =
		{
				.CLI_Command = "run-time-stats", /* The command string to type. */
				.CLI_Command_Description =
						"\r\n run-time-stats: Displays a table showing how much processing time each FreeRTOS task has used\r\n",
				.CLI_Callback = prvRunTimeStatsCommand, /* The function to run. */
		};
#endif /* configGENERATE_RUN_TIME_STATS */

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	/* Structure that defines the "query_heap" command line command. */
CLI_Command_t xQueryHeap =
	{
			.CLI_Command = "query-heap",
			.CLI_Command_Description = "\r\n query-heap: Displays the free heap space, and minimum ever free heap space.\r\n",
			.CLI_Callback = prvQueryHeapCommand, /* The function to run. */
	};
#endif /* configQUERY_HEAP_COMMAND */

#if configUSE_TRACE_FACILITY == 1
/* Structure that defines the "trace" command line command.  This takes a single
 parameter, which can be either "start" or "stop". */
CLI_Command_t xStartStopTrace =
		{
				.CLI_Command = "trace",
				.CLI_Command_Description =
						"\r\n trace [start | stop]: Starts or stops a trace recording for viewing in FreeRTOS+Trace\r\n",
				.CLI_Callback = prvStartStopTraceCommand, /* The function to run. */
		};
#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

/************CLI Commands Definitions ends***********************************/

/************CLI Callback Implementation starts***********************************/

static uint8_t Add_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
		uint16_t max_buffer_len) {

	uint8_t is_command_valid = 1;
	uint16_t param_len = 0;
	uint8_t param_number = 0;
	uint8_t param_count = 0;
	char* param_ptr = NULL;

	int32_t sum = 0;
	int32_t tmp = 0;

	do {

		param_number++; //param number, starting from first

		param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

		if (param_ptr != NULL) {
			param_count++;

			if (Parse_Integer(param_ptr, &tmp)) {
				sum += tmp;
			} else {
				is_command_valid = 0;
			}
		}

	} while (param_ptr != NULL);

	/*at least one parameter is expected*/
	if (!param_count) {
		is_command_valid = 0;
	}

	if (is_command_valid) {

		sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) sum);

	} else {
		Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}
	return 0;
}

static uint8_t Sub_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
		uint16_t max_buffer_len) {

	uint8_t is_command_valid = 1;
	uint16_t param_len = 0;
	uint8_t param_number = 0;
	uint8_t param_count = 0;
	char* param_ptr = NULL;

	int32_t rslt = 0;
	int32_t tmp = 0;

	do {

		param_number++; //param number, starting from first

		param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

		if (param_ptr != NULL) {
			param_count++;

			if (Parse_Integer(param_ptr, &tmp)) {
				/*if first param*/
				if (param_count == 1) {
					rslt = tmp;
				} else {
					rslt -= tmp;
				}
			} else {
				is_command_valid = 0;
			}
		}

	} while (param_ptr != NULL);

	/*at least one parameter is expected*/
	if (!param_count) {
		is_command_valid = 0;
	}

	if (is_command_valid) {

		sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	} else {
		Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

	return 0;

}

static uint8_t Mul_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
		uint16_t max_buffer_len) {

	uint8_t is_command_valid = 1;
	uint16_t param_len = 0;
	uint8_t param_number = 0;
	uint8_t param_count = 0;
	char* param_ptr = NULL;

	int32_t rslt = 1;
	int32_t tmp = 0;

	do {

		param_number++; //param number, starting from first

		param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

		if (param_ptr != NULL) {

			param_count++;

			if (Parse_Integer(param_ptr, &tmp)) {
				rslt *= tmp;
			} else {
				is_command_valid = 0;
			}
		}

	} while (param_ptr != NULL);

	/*at least one parameter is expected*/
	if (!param_count) {
		is_command_valid = 0;
	}

	if (is_command_valid) {

		sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	} else {
		Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

	return 0;

}

static uint8_t Div_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
		uint16_t max_buffer_len) {

	uint8_t is_command_valid = 1;
	uint16_t param_len = 0;
	uint8_t param_number = 0;
	uint8_t param_count = 0;
	char* param_ptr = NULL;

	int32_t rslt = 1;
	int32_t tmp = 0;

	do {

		param_number++; //param number, starting from first

		param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

		if (param_ptr != NULL) {
			param_count++;

			if (Parse_Integer(param_ptr, &tmp)) {
				/*if first param*/
				if (param_count == 1) {
					rslt = tmp;
				} else {
					rslt /= tmp;
				}
			} else {
				is_command_valid = 0;
			}
		}

	} while (param_ptr != NULL);

	/*only two parameters are expected*/
	if (param_count != 2) {
		is_command_valid = 0;
	}

	if (is_command_valid) {

		sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	} else {
		Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

	return 0;

}

static uint8_t Pow_Callback(const char* cli_rx_command, char* cli_tx_out_buffer,
		uint16_t max_buffer_len) {

	uint8_t is_command_valid = 1;
	uint16_t param_len = 0;
	uint8_t param_number = 0;
	uint8_t param_count = 0;
	char* param_ptr = NULL;

	int32_t rslt = 1;
	int32_t tmp = 0;

	do {

		param_number++; //param number, starting from first

		param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

		if (param_ptr != NULL) {
			param_count++;

			if (Parse_Integer(param_ptr, &tmp)) {
				/*if first param*/
				if (param_count == 1) {
					rslt = tmp;
				} else {
					rslt = pow(rslt, tmp);
				}
			} else {
				is_command_valid = 0;
			}
		}

	} while (param_ptr != NULL);

	/*only twh parameters are expected*/
	if (param_count != 2) {
		is_command_valid = 0;
	}

	if (is_command_valid) {

		sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	} else {
		Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

	return 0;

}

static uint8_t Sqrt_Callback(const char* cli_rx_command,
		char* cli_tx_out_buffer, uint16_t max_buffer_len) {

	uint8_t is_command_valid = 1;
	uint16_t param_len = 0;
	uint8_t param_number = 0;
	uint8_t param_count = 0;
	char* param_ptr = NULL;

	int32_t rslt = 1;
	int32_t tmp = 0;

	do {

		param_number++; //param number, starting from first

		param_ptr = CLI_Get_Parameter(cli_rx_command, param_number, &param_len);

		if (param_ptr != NULL) {
			param_count++;
			if (Parse_Integer(param_ptr, &tmp)) {
				rslt = sqrt(tmp);
			} else {
				is_command_valid = 0;
			}
		}

	} while (param_ptr != NULL);

	/*only one parameter is expected*/
	if (param_count != 1) {
		is_command_valid = 0;
	}

	if (is_command_valid) {

		sprintf(cli_tx_out_buffer, "\r\n%i\r\n", (int) rslt);

	} else {
		Sprint_Error_Helper(cli_tx_out_buffer, cli_rx_command);
	}

	return 0;
}

static uint8_t prvTaskStatsCommand(const char *pcCommandString ,char *pcWriteBuffer,
		uint16_t xWriteBufferLen) {
	const char * const pcHeader =
			"State  Priority  Stack   #\r\n************************************************\r\n";
	uint8_t xSpacePadding;

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) pcCommandString;
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	/* Generate a table of task stats. */
	strcpy(pcWriteBuffer, "Task");
	pcWriteBuffer += strlen(pcWriteBuffer);

	/* Minus three for the null terminator and half the number of characters in
	 "Task" so the column lines up with the centre of the heading. */
	configASSERT(configMAX_TASK_NAME_LEN > 3);
	for (xSpacePadding = strlen("Task");
			xSpacePadding < ( configMAX_TASK_NAME_LEN - 3); xSpacePadding++) {
		/* Add a space to align columns after the task's name. */
		*pcWriteBuffer = ' ';
		pcWriteBuffer++;

		/* Ensure always terminated. */
		*pcWriteBuffer = 0x00;
	}
	strcpy(pcWriteBuffer, pcHeader);
	vTaskList(pcWriteBuffer + strlen(pcHeader));

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )

	static uint8_t prvQueryHeapCommand(const char *pcCommandString,
			                           char *pcWriteBuffer,
			                           uint16_t xWriteBufferLen)
	{
		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		sprintf( pcWriteBuffer, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_QUERY_HEAP */
/*-----------------------------------------------------------*/

#if( configGENERATE_RUN_TIME_STATS == 1 )

static uint8_t prvRunTimeStatsCommand(const char *pcCommandString, char *pcWriteBuffer,
		uint16_t xWriteBufferLen) {
	const char * const pcHeader =
			"  Abs Time      % Time\r\n****************************************\r\n";
	uint8_t xSpacePadding;

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) pcCommandString;
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	/* Generate a table of task stats. */
	strcpy(pcWriteBuffer, "Task");
	pcWriteBuffer += strlen(pcWriteBuffer);

	/* Pad the string "task" with however many bytes necessary to make it the
	 length of a task name.  Minus three for the null terminator and half the
	 number of characters in	"Task" so the column lines up with the centre of
	 the heading. */
	for (xSpacePadding = strlen("Task");
			xSpacePadding < ( configMAX_TASK_NAME_LEN - 3); xSpacePadding++) {
		/* Add a space to align columns after the task's name. */
		*pcWriteBuffer = ' ';
		pcWriteBuffer++;

		/* Ensure always terminated. */
		*pcWriteBuffer = 0x00;
	}

	strcpy(pcWriteBuffer, pcHeader);
	vTaskGetRunTimeStats(pcWriteBuffer + strlen(pcHeader));

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

#endif /* configGENERATE_RUN_TIME_STATS */
/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

#if configUSE_TRACE_FACILITY == 1

static uint8_t prvStartStopTraceCommand(const char *pcCommandString,
		                                char *pcWriteBuffer,
		                                uint16_t xWriteBufferLen) {
	const char *pcParameter;
	uint16_t lParameterStringLength;

	/* Remove compile time warnings about unused parameters, and check the
	 write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	 write buffer length is adequate, so does not check for buffer overflows. */
	(void) pcCommandString;
	(void) xWriteBufferLen;
	configASSERT(pcWriteBuffer);

	/* Obtain the parameter string. */
	/* Return the first parameter. */
	pcParameter = CLI_Get_Parameter(pcCommandString,
	                                1,
	                                &lParameterStringLength);

	/* Sanity check something was returned. */
	configASSERT(pcParameter);

	/* There are only two valid parameter values. */
	if (strncmp(pcParameter, "start", strlen("start")) == 0) {
		/* Start or restart the trace. */
		vTraceStop();
		vTraceClear();
		vTraceStart();

		sprintf(pcWriteBuffer, "Trace recording (re)started.\r\n");
	} else if (strncmp(pcParameter, "stop", strlen("stop")) == 0) {
		/* End the trace, if one is running. */
		vTraceStop();
		sprintf(pcWriteBuffer, "Stopping trace recording.\r\n");
	} else {
		sprintf(pcWriteBuffer, "Valid parameters are 'start' and 'stop'.\r\n");
	}

	/* There is no more data to return after this single string, so return
	 pdFALSE. */
	return pdFALSE;
}

#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

/************CLI callback Implementation starts***********************************/

/*
 *
 *
 *
 */
void CLI_Add_All_Commands() {

	CLI_Add_Cammand(&Add_Defination);
	CLI_Add_Cammand(&Sub_Defination);
	CLI_Add_Cammand(&Mul_Defination);
	CLI_Add_Cammand(&Div_Defination);
	CLI_Add_Cammand(&Pow_Defination);
	CLI_Add_Cammand(&Sqrt_Defination);

	/* Register all the command line commands defined immediately above. */
	CLI_Add_Cammand(&xTaskStats);

#if( configGENERATE_RUN_TIME_STATS == 1 )
	{
		CLI_Add_Cammand(&xRunTimeStats);
	}
#endif

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	{
		CLI_Add_Cammand( &xQueryHeap );
	}
	#endif

#if( configUSE_TRACE_FACILITY == 1 )
	{
		CLI_Add_Cammand(&xStartStopTrace);
	}
#endif

	Print_About_Task_Handle = xTaskCreateStatic(Print_About_Task,
			"Print_About_Task",
			PRINT_ABOUT_TASK_STAK_SIZE,
			NULL,
			PRINT_ABOUT_TASK_PRIORITY,
			Print_About_Task_Stack,
			&Print_About_Task_TCB);

}

void Print_About_Task(void* argument) {

	/*** gaurd uart ***/
	xSemaphoreTake(CLI_UART_Mutex_Handle, portMAX_DELAY);

	CLI_UART_Send_String("\r\nSTM32 CLI over uart demo.\r\nCalculator app\r\n");

	/*** release uart ***/
	xSemaphoreGive(CLI_UART_Mutex_Handle);

	vTaskSuspend(Print_About_Task_Handle);

	while (1) {

	}
}
