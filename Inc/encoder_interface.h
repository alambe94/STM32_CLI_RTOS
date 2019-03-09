/*
 * encoder_interface.h
 *
 *  Created on: 08-Mar-2019
 *      Author: medprime
 */

#ifndef ENCODER_INTERFACE_H_
#define ENCODER_INTERFACE_H_


#include "stm32f4xx_hal.h"
#include "motor_config.h"
#include "cli_uart.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "L6470.h"


void Encoder_Loop();

#endif /* ENCODER_INTERFACE_H_ */
