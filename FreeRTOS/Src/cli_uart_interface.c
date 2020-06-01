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
#include "cli.h"
#include "cli_commands.h"
#include "ring_buffer.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/**
 * @addtogroup CLI_Interface
 * @{
 */

/**
 * @defgroup CLI_Interface_Defines
 * @{
 */

#define CLI_UART_TASK_STACK_SIZE 256u
#define CLI_UART_TASK_PRIORITY 5u

#define CLI_RING_BUFFER_SIZE 256
#define OUTPUT_BUFFER_SIZE 128
#define INPUT_BUFFER_SIZE 128

/**
 * @}
 */
/* End of CLI_Interface_Defines */

/**
 * @defgroup CLI_Interface_Macros
 * @{
 */

/**
 * @brief data is written to buffer via uart DMA in background, need to update Write_Index manually
 **/
#define UPDATE_RING_BUFFER() (CLI_Ring_Buffer_Handle.Write_Index = \
                                  (CLI_RING_BUFFER_SIZE -          \
                                   (CLI_UART->hdmarx->Instance->NDTR)))

/**
 * @}
 */
/* End of CLI_Interface_Macros */

/**
 * @defgroup CLI_Interface_externs
 * @{
 */


/**
 * @}
 */
/* End of CLI_Interface_externs */

/**
 * @defgroup CLI_Interface_Gloabl_Variables
 * @{
 */

/**
 * @brief ring buffer for uart reception
 **/
static uint8_t CLI_Ring_Buffer[CLI_RING_BUFFER_SIZE];
static Ring_Buffer_t CLI_Ring_Buffer_Handle;

/**
 * @brief output buffer for cli
 **/
static char CLI_Output_Buffer[OUTPUT_BUFFER_SIZE];

/**
 * @brief input buffer for cli
 **/
static char CLI_CMD_Buffer[INPUT_BUFFER_SIZE];

/**
 * @brief enable or disable echo on command
 **/
static uint8_t Echo_Flag = 0;

/**
 * @brief communication uart
 **/
UART_HandleTypeDef *CLI_UART = &huart2;

/**
 * @brief CLI interface task parameters
 **/
static TaskHandle_t CLI_UART_Task_Handle;
static StackType_t CLI_UART_Task_Stack[CLI_UART_TASK_STACK_SIZE];
static StaticTask_t CLI_UART_Task_TCB;

/**
 * @brief mutex to gaud uart access
 **/
SemaphoreHandle_t CLI_UART_Mutex_Handle;
StaticSemaphore_t CLI_UART_Mutex_Buffer;

/**
 * @brief semaphore to signal transmission complete
 **/
SemaphoreHandle_t CLI_UART_TX_CMPLT_Handle;
StaticSemaphore_t CLI_UART_TX_CMPLT_Buffer;

/**
 * @brief semaphore to signal reception complete or idle interrupt
 **/
SemaphoreHandle_t CLI_UART_RX_CMPLT_Handle;
StaticSemaphore_t CLI_UART_RX_CMPLT_Buffer;

/**
 * @}
 */
/* End of CLI_Interface_Gloabl_Variables */

/**
 * @defgroup CLI_Interface_Protypes
 * @{
 */

/**
 * FreeRTOS task prototype of CLI_UART_Task
*/
static void CLI_UART_Task(void *argument);

/**
 * prototype of echo callback
*/
static uint8_t Echo_Callback(uint8_t argc,
                             const char *argv[],
                             char *cli_out_buffer,
                             uint16_t cli_out_max);

/**
 * @}
 */
/* End of CLI_Interface_Protypes */

/**
 * @defgroup CLI_Interface_Functions
 * @{
 */

/**
 * @brief enable or disable echo on command
 **/
static uint8_t Echo_Callback(uint8_t argc,
                             const char *argv[],
                             char *cli_out_buffer,
                             uint16_t cli_out_max)
{
    if (Echo_Flag)
    {
        Echo_Flag = 0;
        strncpy(CLI_Output_Buffer, "echo disabled\r\n", cli_out_max);
    }
    else
    {
        Echo_Flag = 1;
        strncpy(CLI_Output_Buffer, "echo enabled\r\n", cli_out_max);
    }
    return 0;
}

static CLI_Command_t Echo_Defination =
    {
        /* command string to type */
        .CLI_Command = "echo",

        /* command help string */
        .CLI_Command_Description = "Enable or disable cmd echo",

        /* function to run. */
        .CLI_Callback = Echo_Callback};

/**
 * @brief create mutex to guard uart, create RX and TX complete semaphore
 *        create CLI Interface task and add all commands to CLI
 **/
void CLI_UART_Thread_Add()
{
    CLI_UART_Mutex_Handle = xSemaphoreCreateMutexStatic(&CLI_UART_Mutex_Buffer);

    CLI_UART_TX_CMPLT_Handle = xSemaphoreCreateBinaryStatic(&CLI_UART_TX_CMPLT_Buffer);

    CLI_UART_RX_CMPLT_Handle = xSemaphoreCreateBinaryStatic(&CLI_UART_RX_CMPLT_Buffer);

    CLI_UART_Task_Handle = xTaskCreateStatic(CLI_UART_Task,
                                             "CLI_UART_Task",
                                             CLI_UART_TASK_STACK_SIZE,
                                             NULL,
                                             CLI_UART_TASK_PRIORITY,
                                             CLI_UART_Task_Stack,
                                             &CLI_UART_Task_TCB);

    CLI_Init();
    CLI_Add_Command(&Echo_Defination);
    CLI_Add_All_Commands();
}

/**
 * @brief send single character to uart
 * @note must acquire uart mutex before calling this funtion
 **/
void CLI_UART_Send_Char(char data)
{
    CLI_UART->Instance->DR = (data);
    while (__HAL_UART_GET_FLAG(CLI_UART, UART_FLAG_TC) == 0)
        ;
}

/**
 * @brief send string to uart
 * @note must acquire uart mutex before calling this funtion
 **/
void CLI_UART_Send_String(char *data)
{
    while (*data)
    {
        CLI_UART_Send_Char(*data++);
    }
}

/**
 * @brief send string to uart using DMA
 * @note must acquire uart mutex before calling this funtion
 **/
void CLI_UART_Send_String_DMA(const char *data)
{
    uint16_t len = strlen(data);
    HAL_UART_Transmit_DMA(CLI_UART, (uint8_t *)data, len);
}

/**
 * @brief convert integer to string and send to uart
 * @note must acquire uart mutex before calling this funtion
 **/
void CLI_UART_Send_Int(int32_t num)
{
    char int_to_str[10] = "";
    itoa(num, int_to_str, 10);
    CLI_UART_Send_String(int_to_str);
}

/**
 * @brief convert float to string and send to uart
 * @note must acquire uart mutex before calling this funtion
 **/
void CLI_UART_Send_Float(float num)
{
    char int_to_str[10] = "";
    sprintf(int_to_str, "%0.2f", num);
    CLI_UART_Send_String(int_to_str);
}

/**
 * @brief FreeRTOS CLI uart task
 * @param  argument FreeRTOS input argument
 **/
static void CLI_UART_Task(void *argument)
{
    /**
     * @note need to be static if not using RTOS
     **/
    //static uint8_t rx_char_count;
    uint8_t rx_char_count = 0; //
    uint8_t call_again = 0;
    uint8_t rx_char = 0;

    Ring_Buffer_Init(&CLI_Ring_Buffer_Handle,
                     CLI_Ring_Buffer,
		     1,
                     CLI_RING_BUFFER_SIZE);

    HAL_UART_Receive_DMA(CLI_UART,
                         CLI_Ring_Buffer,
                         CLI_RING_BUFFER_SIZE);

    /* Enable idle interrupt */
    __HAL_UART_ENABLE_IT(CLI_UART, UART_IT_IDLE);

    /* gaurd uart */
    xSemaphoreTake(CLI_UART_Mutex_Handle, portMAX_DELAY);

    for (;;)
    {
        /* release uart */
        xSemaphoreGive(CLI_UART_Mutex_Handle);

        /* wait for idle line interrupt */
        xSemaphoreTake(CLI_UART_RX_CMPLT_Handle, portMAX_DELAY);

        /* gaurd uart */
        xSemaphoreTake(CLI_UART_Mutex_Handle, portMAX_DELAY);

        /* go through every character in ring buffer and build input string if '\r' is found then pass the string to CLI_Processer */
        while (Ring_Buffer_Get_Count(&CLI_Ring_Buffer_Handle))
        {
            /* pop one char from buffer */
            Ring_Buffer_Get_Char(&CLI_Ring_Buffer_Handle, &rx_char);

            /* if echo flag is set then echo back received char */
            if (Echo_Flag)
            {
                CLI_UART_Send_Char(rx_char);
            }

            /* string complete */
            if (rx_char == '\r')
            {
                /* check '\n'. normally '\r' is followed by '\n' */
                Ring_Buffer_Get_Char(&CLI_Ring_Buffer_Handle, &rx_char);

                /* if echo flag is set then echo back received char */
                if (Echo_Flag)
                {
                    CLI_UART_Send_Char(rx_char);
                }

                /* reset CLI_CMD_Buffer index */
                rx_char_count = 0;

                /* reset ring buffer */
                Ring_Buffer_Flush(&CLI_Ring_Buffer_Handle);

                /* process command, call it repeatedly until it returns 0*/
                do
                {
                    /* reset output buffer for strlen to work properly just in case */
                    memset(CLI_Output_Buffer, 0x00, OUTPUT_BUFFER_SIZE);

                    call_again = CLI_Process_Command(CLI_CMD_Buffer,
                                                     CLI_Output_Buffer,
                                                     OUTPUT_BUFFER_SIZE);

                    if (CLI_Output_Buffer[0] != '\0')
                    {
                        /* send output to console */
                        CLI_UART_Send_String_DMA(CLI_Output_Buffer);

                        /* wait for transmission to complete */
                        xSemaphoreTake(CLI_UART_TX_CMPLT_Handle, portMAX_DELAY);
                    }
                } while (call_again);

                /* send console delimiter */
                CLI_UART_Send_String("->");
            }
            else
            {
                /* build input string */
                if (!rx_char_count)
                {
                    /* reset command buffer if building new string */
                    memset(CLI_CMD_Buffer, 0x00, INPUT_BUFFER_SIZE);
                }

                /* backspace or delete */
                if ((rx_char == '\b') || (rx_char == 0x7F))
                {
                    /* Backspace was pressed. Erase the last character in the string - if any. */
                    if (rx_char_count)
                    {
                        rx_char_count--;
                        CLI_CMD_Buffer[rx_char_count] = '\0';
                    }
                }
                else
                {
                    /* accumulate chars in input buffer */
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
}

/**
 * @brief uart TX ISR , gives tx complete semaphore
 * @see HAL_UART_TxCpltCallback
 **/
void CLI_UART_DMA_TX_Done()
{
    BaseType_t xHigherPriorityTaskWoken;
    xSemaphoreGiveFromISR(CLI_UART_TX_CMPLT_Handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief uart RX ISR , gives rx complete semaphore
 * @see USART2_IRQHandler
 **/
void CLI_UART_RX_ISR()
{
    if (__HAL_UART_GET_FLAG(CLI_UART, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(CLI_UART);

        /* data is written to buffer via uart DMA in background*/
        /* need to update Write_Index manually */
        UPDATE_RING_BUFFER();

        BaseType_t xHigherPriorityTaskWoken;
        xSemaphoreGiveFromISR(CLI_UART_RX_CMPLT_Handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
 * @brief emulate uart RX ISR from usb cdc, gives rx complete semaphore
 * @see CDC_Receive_FS
 **/
void CDC_Receive_FS_ISR(uint8_t *buff, uint32_t len)
{
    while (len--)
    {
        Ring_Buffer_Put_Char(&CLI_Ring_Buffer_Handle, (*buff++));

        CLI_UART->hdmarx->Instance->NDTR--;
        if (CLI_UART->hdmarx->Instance->NDTR == 0)
        {
            CLI_UART->hdmarx->Instance->NDTR = CLI_RING_BUFFER_SIZE;
        }
    }

    BaseType_t xHigherPriorityTaskWoken;
    xSemaphoreGiveFromISR(CLI_UART_RX_CMPLT_Handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @}
 */
/* End of CLI_Interface_Functions */

/**
 * @}
 */
/* End of CLI_Interface */
