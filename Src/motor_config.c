/*
 * motor_config.c
 *
 *  Created on: 05-Mar-2019
 *      Author: medprime
 */

#include "motor_config.h"



#define MAX_X_AXIS_STEPS (1.875*200*128)
#define MAX_Y_AXIS_STEPS (0.65625*200*128)
#define MAX_Z_AXIS_STEPS (37.5*200*128)
#define MAX_M_AXIS_STEPS 6000


StepperMotorDriverHandle_t Motor_X_Handler;
StepperMotorDriverHandle_t Motor_Y_Handler;
StepperMotorDriverHandle_t Motor_Z_Handler;
StepperMotorDriverHandle_t Motor_M_Handler;

MotorParameterData_t Motor_X_Data;
MotorParameterData_t Motor_Y_Data;
MotorParameterData_t Motor_Z_Data;
MotorParameterData_t Motor_M_Data;



uint8_t  Control_Mode = 0;

uint8_t  Motor_Direction[NO_OF_MOTORS] ={0};

int32_t Motor_Parameter[NO_OF_MOTORS] ={0}; // speed steps
int32_t Motor_Current_Steps[NO_OF_MOTORS] ={0}; // speed steps
int32_t Motor_MAX_Steps[NO_OF_MOTORS] ={MAX_X_AXIS_STEPS,MAX_Y_AXIS_STEPS,MAX_Z_AXIS_STEPS,MAX_M_AXIS_STEPS}; // speed steps


void Motor_Config()
    {

    Motor_X_Data.motorvoltage = 12.0;
    Motor_X_Data.fullstepsperrevolution = 200;
    Motor_X_Data.phasecurrent = 1.5;
    Motor_X_Data.phasevoltage = 3.0;
    Motor_X_Data.speed = 200.0;
    Motor_X_Data.acc = 100.0;
    Motor_X_Data.dec = 50.0;
    Motor_X_Data.maxspeed = 6000.0;
    Motor_X_Data.minspeed = 0.0;
    Motor_X_Data.fsspd = 1000.0;
    Motor_X_Data.kvalhold = 3.06;
    Motor_X_Data.kvalrun = 3.06;
    Motor_X_Data.kvalacc = 3.06;
    Motor_X_Data.kvaldec = 3.06;
    Motor_X_Data.intspeed = 61.52;
    Motor_X_Data.stslp = 392.1569e-6;
    Motor_X_Data.fnslpacc = 643.1372e-6;
    Motor_X_Data.fnslpdec = 643.1372e-6;
    Motor_X_Data.kterm = 0;
    Motor_X_Data.ocdth =   1500 * 1.00;
    Motor_X_Data.stallth = 1200 * 1.00;
    Motor_X_Data.step_sel = MICROSTEP_1_128;
    Motor_X_Data.alarmen = 0xFF;
    Motor_X_Data.config = 0x2E88;


    Motor_Y_Data.motorvoltage = 12.0;
    Motor_Y_Data.fullstepsperrevolution = 200;
    Motor_Y_Data.phasecurrent = 1.5;
    Motor_Y_Data.phasevoltage = 3.0;
    Motor_Y_Data.speed = 100.0;
    Motor_Y_Data.acc = 100.0;
    Motor_Y_Data.dec = 50.0;
    Motor_Y_Data.maxspeed = 6000.0;
    Motor_Y_Data.minspeed = 0.0;
    Motor_Y_Data.fsspd = 602.7;
    Motor_Y_Data.kvalhold = 3.06;
    Motor_Y_Data.kvalrun = 3.06;
    Motor_Y_Data.kvalacc = 3.06;
    Motor_Y_Data.kvaldec = 3.06;
    Motor_Y_Data.intspeed = 61.52;
    Motor_Y_Data.stslp = 392.1569e-6;
    Motor_Y_Data.fnslpacc = 643.1372e-6;
    Motor_Y_Data.fnslpdec = 643.1372e-6;
    Motor_Y_Data.kterm = 0;
    Motor_Y_Data.ocdth = 1 * 1500 * 1.00;
    Motor_Y_Data.stallth = 1000 * 1.00;
    Motor_Y_Data.step_sel = MICROSTEP_1_128;
    Motor_Y_Data.alarmen = 0xFF;
    Motor_Y_Data.config = 0x2E88;

    Motor_Z_Data.motorvoltage = 12.0;
    Motor_Z_Data.fullstepsperrevolution = 200;
    Motor_Z_Data.phasecurrent = 1.5;
    Motor_Z_Data.phasevoltage = 3.0;
    Motor_Z_Data.speed = 100.0;
    Motor_Z_Data.acc = 500.0;
    Motor_Z_Data.dec = 250.0;
    Motor_Z_Data.maxspeed = 6000.0;
    Motor_Z_Data.minspeed = 100.0;
    Motor_Z_Data.fsspd = 602.7;
    Motor_Z_Data.kvalhold = 3.06;
    Motor_Z_Data.kvalrun = 3.06;
    Motor_Z_Data.kvalacc = 3.06;
    Motor_Z_Data.kvaldec = 3.06;
    Motor_Z_Data.intspeed = 61.52;
    Motor_Z_Data.stslp = 392.1569e-6;
    Motor_Z_Data.fnslpacc = 643.1372e-6;
    Motor_Z_Data.fnslpdec = 643.1372e-6;
    Motor_Z_Data.kterm = 0;
    Motor_Z_Data.ocdth = 1 * 1500 * 1.00;
    Motor_Z_Data.stallth = 1000 * 1.00;
    Motor_Z_Data.step_sel = MICROSTEP_1_128;
    Motor_Z_Data.alarmen = 0xFF;
    Motor_Z_Data.config = 0x2E88;

    Motor_M_Data.motorvoltage = 12.0;
    Motor_M_Data.fullstepsperrevolution = 200;
    Motor_M_Data.phasecurrent = 1.0;
    Motor_M_Data.phasevoltage = 3.0;
    Motor_M_Data.speed = 1000.0;
    Motor_M_Data.acc = 100.0;
    Motor_M_Data.dec = 50.0;
    Motor_M_Data.maxspeed = 6000.0;
    Motor_M_Data.minspeed = 100.0;
    Motor_M_Data.fsspd = 800.0;
    Motor_M_Data.kvalhold = 3.06;
    Motor_M_Data.kvalrun = 3.06;
    Motor_M_Data.kvalacc = 3.06;
    Motor_M_Data.kvaldec = 3.06;
    Motor_M_Data.intspeed = 61.52;
    Motor_M_Data.stslp = 392.1569e-6;
    Motor_M_Data.fnslpacc = 643.1372e-6;
    Motor_M_Data.fnslpdec = 643.1372e-6;
    Motor_M_Data.kterm = 0;
    Motor_M_Data.ocdth =   2000 * 1.00;
    Motor_M_Data.stallth = 1500 * 1.00;
    Motor_M_Data.step_sel = MICROSTEP_1_16;
    Motor_M_Data.alarmen = 0xFF;
    Motor_M_Data.config = 0x2E88;

    L6470_ENABLE();

    L6470_ResetDevice(0);
    L6470_GetStatus(0);

    L6470_ResetDevice(1);
    L6470_GetStatus(1);

    L6470_ResetDevice(2);
    L6470_GetStatus(2);

    L6470_ResetDevice(3);
    L6470_GetStatus(3);

    Motor_X_Handler.DaisyChainPosition = 0;
    Motor_X_Handler.Command = &L6470Command;
    L6470_Config(&Motor_X_Handler, &Motor_X_Data);

    Motor_Y_Handler.DaisyChainPosition = 1;
    Motor_Y_Handler.Command = &L6470Command;
    L6470_Config(&Motor_Y_Handler, &Motor_Y_Data);

    Motor_Z_Handler.DaisyChainPosition = 2;
    Motor_Z_Handler.Command = &L6470Command;
    L6470_Config(&Motor_Z_Handler, &Motor_Z_Data);

    Motor_M_Handler.DaisyChainPosition = 3;
    Motor_M_Handler.Command = &L6470Command;
    L6470_Config(&Motor_M_Handler, &Motor_M_Data);


    uint16_t status_register_x = L6470_GetStatus(0);
    uint16_t status_register_y = L6470_GetStatus(1);
    uint16_t status_register_z = L6470_GetStatus(2);
    uint16_t status_register_m = L6470_GetStatus(3);

    (void)status_register_m;

    if(status_register_x & STATUS_SW_F)
	{
	L6470_PrepareReleaseSW(0, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	}
    else
	{
	    L6470_PrepareGoUntil(0,
		    L6470_ACT_RST_ID,
		    L6470_DIR_REV_ID,
		    6000);
	}

    if(status_register_y & STATUS_SW_F)
	{
	L6470_PrepareReleaseSW(1, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	}
    else
	{
	    L6470_PrepareGoUntil(1,
		    L6470_ACT_RST_ID,
		    L6470_DIR_REV_ID,
		    6000);
	}

    if(status_register_z & STATUS_SW_F)
	{
	L6470_PrepareReleaseSW(2, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	}
    else
	{
	    L6470_PrepareGoUntil(2,
		    L6470_ACT_RST_ID,
		    L6470_DIR_REV_ID,
		    6000);
	}

    L6470_PerformPreparedApplicationCommand();

    }

