/*
 * encoder_interface.h
 *
 *  Created on: 08-Mar-2019
 *      Author: medprime
 */

#ifndef ENCODER_INTERFACE_H_
#define ENCODER_INTERFACE_H_


#include "cli_uart_interface.h"
#include "stm32f4xx_hal.h"
#include "motor_config.h"
#include "math.h"
#include "stdlib.h"
#include "L6470.h"

void Encoder_Interface_Config();
void Encoder_Loop();

#endif /* ENCODER_INTERFACE_H_ */
