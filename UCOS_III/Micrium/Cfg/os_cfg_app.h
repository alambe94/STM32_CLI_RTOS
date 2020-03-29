/*
*********************************************************************************************************
*                                               uC/OS-III
*                                          The Real-Time Kernel
*
*                         (c) Copyright 2009-2018; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                               OS CONFIGURATION (APPLICATION SPECIFICS)
*
* Filename : os_cfg_app.h
* Version  : V3.07.03
*********************************************************************************************************
*/

#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H

/*
************************************************************************************************************************
*                                                      CONSTANTS
************************************************************************************************************************
*/
                                                                /* ------------------ MISCELLANEOUS ------------------- */
#define  OS_CFG_ISR_STK_SIZE                         128u       /* Stack size of ISR stack (number of CPU_STK elements) */

#define  OS_CFG_MSG_POOL_SIZE                         32u       /* Maximum number of messages                           */

#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY              10u       /* Stack limit position in percentage to empty          */


                                                                /* -------------------- IDLE TASK --------------------- */
#define  OS_CFG_IDLE_TASK_STK_SIZE                    64u       /* Stack size (number of CPU_STK elements)              */


                                                                /* ------------------ STATISTIC TASK ------------------ */
#define  OS_CFG_STAT_TASK_PRIO  ((OS_PRIO)(OS_CFG_PRIO_MAX-2u)) /* Priority                                             */
#define  OS_CFG_STAT_TASK_RATE_HZ                     10u       /* Rate of execution (1 to 10 Hz)                       */
#define  OS_CFG_STAT_TASK_STK_SIZE                   100u       /* Stack size (number of CPU_STK elements)              */


                                                                /* ---------------------- TICKS ----------------------- */
#define  OS_CFG_TICK_RATE_HZ                        1000u       /* Tick rate in Hertz (10 to 1000 Hz)                   */


                                                                /* --------------------- TIMERS ----------------------- */
#define  OS_CFG_TMR_TASK_PRIO   ((OS_PRIO)(OS_CFG_PRIO_MAX-3u)) /* Priority of 'Timer Task'                             */
#define  OS_CFG_TMR_TASK_STK_SIZE                    128u       /* Stack size (number of CPU_STK elements)              */

#define  OS_CFG_TMR_TASK_RATE_HZ                      10u       /* DEPRECATED - Rate for timers (10 Hz Typ.)            */
                                                                /* The timer task now calculates its timeouts based     */
                                                                /* on the timers in the list. It no longer runs at a    */
                                                                /* static frequency.                                    */
                                                                /* This define is included for compatibility reasons.   */
                                                                /* It will determine the period of a timer tick.        */
                                                                /* We recommend setting it to OS_CFG_TICK_RATE_HZ       */
                                                                /* for new projects.                                    */


#endif
