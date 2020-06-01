/*
 * CLI_commands.c
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#include "cli_commands.h"
#include "cli_uart_interface.h"
#include "printf.h"
#include "cli.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define PRINT_ABOUT_TASK_STAK_SIZE 128u
#define PRINT_ABOUT_TASK_PRIORITY 2u
TaskHandle_t Print_About_Task_Handle;
StackType_t Print_About_Task_Stack[PRINT_ABOUT_TASK_STAK_SIZE];
StaticTask_t Print_About_Task_TCB;
static void Print_About_Task(void *argument);

extern SemaphoreHandle_t CLI_UART_Mutex_Handle;

#define configINCLUDE_QUERY_HEAP_COMMAND 1

/*************************************************************************/
void Sprint_Ok_Helper(const char *in, char *out, uint16_t out_max)
{
    snprintf(out, out_max, "\r\nOk:%s\r\n", in);
}

void Sprint_Error_Helper(const char *in, char *out, uint16_t out_max)
{
    snprintf(out, out_max, "\r\nError:CMD invalid->%s\r\n", in);
}

void Sprint_Ok_Done_Helper(const char *in, char *out, uint16_t out_max)
{
    snprintf(out, out_max, "\r\nOk:%s\r\nDone\r\n", in);
}

/*
 *
 */
uint8_t Parse_Integer(const char *param, int32_t *value)
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
static uint8_t Add_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max);

static uint8_t Sub_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max);

static uint8_t Mul_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max);

static uint8_t Div_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max);

static uint8_t Pow_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max);

static uint8_t Sqrt_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                             uint16_t out_max);

/*
 * Implements the task-stats command.
 */
static uint8_t Task_Stats_Callback(uint8_t argc, const char *argv[],
                                   char *out_buffer, uint16_t out_max);

/*
 * Implements the run-time-stats command.
 */
#if (configGENERATE_RUN_TIME_STATS == 1)
static uint8_t Run_Time_Stats_Callabck(uint8_t argc, const char *argv[],
                                       char *out_buffer, uint16_t out_max);
#endif /* configGENERATE_RUN_TIME_STATS */

/*
 * Implements the "query heap" command.
 */
#if (configINCLUDE_QUERY_HEAP_COMMAND == 1)
static uint8_t Get_Heap_Callback(uint8_t argc, const char *argv[],
                                 char *out_buffer, uint16_t out_max);
#endif

/*
 * Implements the "trace start" and "trace stop" commands;
 */
#if (configUSE_TRACE_FACILITY == 1)
static uint8_t Start_Stop_Trace_Callback(uint8_t argc, const char *argv[],
                                         char *out_buffer, uint16_t out_maxn);
#endif
/************CLI Callback Prototypes Ends***********************************/

/************CLI Commands Definitions starts***********************************/

CLI_Command_t Add_Definition =
    {
        .CLI_Command = "add",
        .CLI_Command_Description = "addition",
        .CLI_Callback = Add_Callback,
};

CLI_Command_t Sub_Definition =
    {
        .CLI_Command = "sub",
        .CLI_Command_Description = "subtraction",
        .CLI_Callback = Sub_Callback,
};

CLI_Command_t Mul_Definition =
    {
        .CLI_Command = "mul",
        .CLI_Command_Description = "multiplication",
        .CLI_Callback = Mul_Callback,
};

CLI_Command_t Div_Definition =
    {
        .CLI_Command = "div",
        .CLI_Command_Description = "division",
        .CLI_Callback = Div_Callback,
};

CLI_Command_t Pow_Definition =
    {
        .CLI_Command = "pow",
        .CLI_Command_Description = "raise power",
        .CLI_Callback = Pow_Callback,
};

CLI_Command_t Sqrt_Definition =
    {
        .CLI_Command = "sqrt",
        .CLI_Command_Description = "square root",
        .CLI_Callback = Sqrt_Callback,
};

/* Structure that defines the "task-stats" command line command.  This generates
 a table that gives information on each task in the system. */
CLI_Command_t xTaskStats =
    {
        .CLI_Command = "task-stats",
        .CLI_Command_Description =
            "task-stats: Displays a table showing the state of each FreeRTOS task",
        .CLI_Callback = Task_Stats_Callback,
};

#if (configGENERATE_RUN_TIME_STATS == 1)
/* Structure that defines the "run-time-stats" command line command.   This
 generates a table that shows how much run time each task has */
CLI_Command_t xRunTimeStats =
    {
        .CLI_Command = "run-time-stats",
        .CLI_Command_Description =
            "run-time-stats: Displays a table showing how much processing time each FreeRTOS task has used.",
        .CLI_Callback = Run_Time_Stats_Callabck,
};
#endif /* configGENERATE_RUN_TIME_STATS */

#if (configINCLUDE_QUERY_HEAP_COMMAND == 1)
/* Structure that defines the "query_heap" command line command. */
CLI_Command_t xQueryHeap =
    {
        .CLI_Command = "query-heap",
        .CLI_Command_Description =
            "query-heap: Displays the free heap space, and minimum ever free heap space.",
        .CLI_Callback = Get_Heap_Callback,
};
#endif /* configQUERY_HEAP_COMMAND */

#if configUSE_TRACE_FACILITY == 1
/* Structure that defines the "trace" command line command.  This takes a single
 parameter, which can be either "start" or "stop". */
CLI_Command_t xStartStopTrace =
    {
        .CLI_Command = "trace",
        .CLI_Command_Description =
            "trace [start | stop]: Starts or stops a trace recording for viewing in FreeRTOS+Trace",
        .CLI_Callback = Start_Stop_Trace_Callback,
};
#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

/************CLI Commands Definitions ends***********************************/

/************CLI Callback Implementation starts***********************************/

static uint8_t Add_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    rslt += tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%li\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Sub_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    rslt -= tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%li\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Mul_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    rslt *= tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%li\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Div_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc > 2)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            for (uint8_t i = 2; i < argc; i++)
            {
                if (Parse_Integer(argv[i], &tmp))
                {
                    if (tmp == 0)
                    {
                        snprintf(out_buffer, out_max,
                                 "divide by zero error, %d argument is zero",
                                 i - 1);
                        return 0;
                    }
                    rslt *= tmp;
                }
                else
                {
                    is_command_valid = 0;
                }
            }
        }
        else
        {
            is_command_valid = 0;
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid)
    {
        snprintf(out_buffer, out_max, "\r\n%li\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Pow_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                            uint16_t out_max)
{
    uint8_t is_command_valid = 1;
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (argc == 3)
    {
        if (Parse_Integer(argv[1], &rslt))
        {
            if (Parse_Integer(argv[2], &tmp))
            {
                rslt = pow(rslt, tmp);
            }
            else
            {
                is_command_valid = 0;
            }
        }
    }
    else
    {
        is_command_valid = 0;
    }

    if (is_command_valid && argc)
    {
        snprintf(out_buffer, out_max, "\r\n%li\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Sqrt_Callback(uint8_t argc, const char *argv[], char *out_buffer,
                             uint16_t out_max)
{
    int32_t rslt = 0;
    int32_t tmp = 0;

    if (Parse_Integer(argv[1], &tmp) && argc > 1)
    {
        rslt = sqrt(tmp);
        snprintf(out_buffer, out_max, "\r\n%li\r\n", rslt);
    }
    else
    {
        Sprint_Error_Helper(argv[0], out_buffer, out_max);
    }

    return 0;
}

static uint8_t Task_Stats_Callback(uint8_t argc, const char *argv[],
                                   char *out_buffer, uint16_t out_max)
{
    const char *const pcHeader =
        "State  Priority  Stack   #\r\n************************************************\r\n";
    uint8_t xSpacePadding;

    /* Generate a table of task stats. */
    strcpy(out_buffer, "Task");
    out_buffer += strlen(out_buffer);

    /* Minus three for the null terminator and half the number of characters in
     "Task" so the column lines up with the centre of the heading. */
    configASSERT(configMAX_TASK_NAME_LEN > 3);
    for (xSpacePadding = strlen("Task");
         xSpacePadding < (configMAX_TASK_NAME_LEN - 3); xSpacePadding++)
    {
        /* Add a space to align columns after the task's name. */
        *out_buffer = ' ';
        out_buffer++;

        /* Ensure always terminated. */
        *out_buffer = 0x00;
    }
    strcpy(out_buffer, pcHeader);
    vTaskList(out_buffer + strlen(pcHeader));

    /* There is no more data to return after this single string, so return
     pdFALSE. */
    return pdFALSE;
}
/*-----------------------------------------------------------*/

#if (configINCLUDE_QUERY_HEAP_COMMAND == 1)

static uint8_t Get_Heap_Callback(uint8_t argc, const char *argv[],
                                 char *out_buffer, uint16_t out_max)
{
    sprintf(out_buffer,
            "Current free heap %d bytes, minimum ever free heap %d bytes\r\n",
            (int)xPortGetFreeHeapSize(),
            (int)xPortGetMinimumEverFreeHeapSize());

    /* There is no more data to return after this single string, so return
     pdFALSE. */
    return pdFALSE;
}

#endif /* configINCLUDE_QUERY_HEAP */
/*-----------------------------------------------------------*/

#if (configGENERATE_RUN_TIME_STATS == 1)

static uint8_t Run_Time_Stats_Callabck(uint8_t argc, const char *argv[],
                                       char *out_buffer, uint16_t out_max)
{
    const char *const pcHeader =
        "  Abs Time      % Time\r\n****************************************\r\n";
    uint8_t xSpacePadding;

    /* Generate a table of task stats. */
    strcpy(out_buffer, "Task");
    out_buffer += strlen(out_buffer);

    /* Pad the string "task" with however many bytes necessary to make it the
     length of a task name.  Minus three for the null terminator and half the
     number of characters in	"Task" so the column lines up with the centre of
     the heading. */
    for (xSpacePadding = strlen("Task");
         xSpacePadding < (configMAX_TASK_NAME_LEN - 3); xSpacePadding++)
    {
        /* Add a space to align columns after the task's name. */
        *out_buffer = ' ';
        out_buffer++;

        /* Ensure always terminated. */
        *out_buffer = 0x00;
    }

    strcpy(out_buffer, pcHeader);
    vTaskGetRunTimeStats(out_buffer + strlen(pcHeader));

    /* There is no more data to return after this single string, so return
     pdFALSE. */
    return pdFALSE;
}

#endif /* configGENERATE_RUN_TIME_STATS */
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

#if configUSE_TRACE_FACILITY == 1

static uint8_t Start_Stop_Trace_Callback(uint8_t argc, const char *argv[],
                                         char *out_buffer, uint16_t out_max)
{

    /* There are only two valid parameter values. */
    if (strncmp(argv[1], "start", strlen("start")) == 0)
    {
        /* Start or restart the trace. */
        vTraceStop();
        vTraceClear();
        vTraceStart();

        sprintf(out_buffer, "Trace recording (re)started.\r\n");
    }
    else if (strncmp(argv[1], "stop", strlen("stop")) == 0)
    {
        /* End the trace, if one is running. */
        vTraceStop();
        sprintf(out_buffer, "Stopping trace recording.\r\n");
    }
    else
    {
        sprintf(out_buffer, "Valid parameters are 'start' and 'stop'.\r\n");
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
void CLI_Add_All_Commands()
{
    CLI_Add_Command(&Add_Definition);
    CLI_Add_Command(&Sub_Definition);
    CLI_Add_Command(&Mul_Definition);
    CLI_Add_Command(&Div_Definition);
    CLI_Add_Command(&Pow_Definition);
    CLI_Add_Command(&Sqrt_Definition);

    CLI_Add_Command(&xTaskStats);

#if (configGENERATE_RUN_TIME_STATS == 1)
    {
        CLI_Add_Command(&xRunTimeStats);
    }
#endif

#if (configINCLUDE_QUERY_HEAP_COMMAND == 1)
    {
        CLI_Add_Command(&xQueryHeap);
    }
#endif

#if (configUSE_TRACE_FACILITY == 1)
    {
        CLI_Add_Command(&xStartStopTrace);
    }
#endif

    Print_About_Task_Handle = xTaskCreateStatic(Print_About_Task,
                                                "Print_About_Task",
                                                PRINT_ABOUT_TASK_STAK_SIZE,
                                                NULL,
                                                PRINT_ABOUT_TASK_PRIORITY, Print_About_Task_Stack,
                                                &Print_About_Task_TCB);
}

void Print_About_Task(void *argument)
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
