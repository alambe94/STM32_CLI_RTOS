/*
 * ucos_main.c
 *
 *  Created on: May 11, 2019
 *      Author: medprime
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "os_cfg_app.h"
#include "lib_mem.h"
#include "os.h"



#define  APP_CFG_STARTUP_TASK_PRIO          3u
#define  APP_CFG_STARTUP_TASK_STK_SIZE    128u
static  OS_TCB   StartupTaskTCB;
static  CPU_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
static  void  StartupTask (void  *p_arg);

//#define TRC_USE_TRACEALYZER_RECORDER 1

int  ucos_main (void)
{


#if (TRC_USE_TRACEALYZER_RECORDER == 1)
  vTraceEnable(TRC_START);
#endif

    OS_ERR  os_err;

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    CPU_IntDis();                                               /* Disable all Interrupts                               */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

    OSInit(&os_err);                                            /* Initialize uC/OS-III                                 */
    if (os_err != OS_ERR_NONE) {
        while (1);
    }

    OSTaskCreate(&StartupTaskTCB,                               /* Create the startup task                              */
                 "Startup Task",
                  StartupTask,
                  0u,
                  APP_CFG_STARTUP_TASK_PRIO,
                  &StartupTaskStk[0u],
                  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE / 10u],
                  APP_CFG_STARTUP_TASK_STK_SIZE,
                  0u,
                  0u,
                  0u,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                  &os_err);
    if (os_err != OS_ERR_NONE) {
        while (1);
    }

    OSStart(&os_err);                                           /* Start multitasking (i.e. give control to uC/OS-III)  */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}

extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);
extern void MX_DMA_Init(void);
extern void MX_USART2_UART_Init(void);
extern void CLI_UART_Thread_Add();


extern UART_HandleTypeDef huart2;


static  void  StartupTask (void *p_arg)
{
    OS_ERR  os_err;


   (void)p_arg;

   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
   HAL_Init();

   /* Configure the system clock */
   SystemClock_Config();

   /*config systic to generate tick interrupt*/
   HAL_SYSTICK_Config(SystemCoreClock /OS_CFG_TICK_RATE_HZ);

   /*set systic interrupt priority to lowest */
   HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0U);

   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   MX_DMA_Init();
   MX_USART2_UART_Init();

   CLI_UART_Thread_Add();

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        OSTimeDlyHMSM(0u, 0u, 0u, 100u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
    }
}





