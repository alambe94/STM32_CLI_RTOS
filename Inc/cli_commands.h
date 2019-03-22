/*
 * CLI_commands.h
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#ifndef CLI_COMMANDS_H_
#define CLI_COMMANDS_H_

#include "stm32f4xx_hal.h"
#include "freeRTOS_cli.h"
#include "cli_uart_interface.h"
#include "motor_config.h"
#include "stdio.h"
#include "stdlib.h"
#include "L6470.h"
#include "i2c.h"


void CLI_Add_All_Commands();
void L6470_Loop();


#endif /* CLI_COMMANDS_H_ */
