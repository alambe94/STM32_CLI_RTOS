/*
 * encoder_interface.c
 *
 *  Created on: 08-Mar-2019
 *      Author: medprime
 */
#include "encoder_interface.h"

extern TIM_HandleTypeDef htim2;

extern int32_t Motor_Current_Steps[NO_OF_MOTORS]; // speed steps
extern int32_t Motor_MAX_Steps[NO_OF_MOTORS];
extern int32_t Motor_Parameter[NO_OF_MOTORS]; // speed steps
extern uint8_t Motor_Direction[NO_OF_MOTORS];
extern uint8_t Control_Mode;

void Encoder_Loop()
    {

    static uint32_t time_stamp = 0;
    static uint8_t  run_flag = 0;
    int32_t speed_val;
    char int_to_str[15];

    if (Control_Mode == 0)
	{

	int16_t encoder_count_x = htim2.Instance->CNT;

	if (HAL_GetTick() - time_stamp > 5)
	    {

	    time_stamp = HAL_GetTick();

	    if (encoder_count_x != 0)
		{
		    L6470_PrepareGetParam(X_AXIS_INDEX,L6470_ABS_POS_ID);
		    L6470_PrepareGetParam(Y_AXIS_INDEX,L6470_ABS_POS_ID);
		    L6470_PrepareGetParam(Z_AXIS_INDEX,L6470_ABS_POS_ID);
		    L6470_PrepareGetParam(M_AXIS_INDEX,L6470_ABS_POS_ID);
		    L6470_PerformPreparedApplicationCommand();

		    Motor_Current_Steps[X_AXIS_INDEX] = L6470_ExtractReturnedData(X_AXIS_INDEX, (uint8_t*)L6470_DaisyChainSpiRxStruct, 3);
		    Motor_Current_Steps[Y_AXIS_INDEX] = L6470_ExtractReturnedData(Y_AXIS_INDEX, (uint8_t*)L6470_DaisyChainSpiRxStruct, 3);
		    Motor_Current_Steps[Z_AXIS_INDEX] = L6470_ExtractReturnedData(Z_AXIS_INDEX, (uint8_t*)L6470_DaisyChainSpiRxStruct, 3);
		    Motor_Current_Steps[M_AXIS_INDEX] = L6470_ExtractReturnedData(M_AXIS_INDEX, (uint8_t*)L6470_DaisyChainSpiRxStruct, 3);


		    Motor_Current_Steps[X_AXIS_INDEX] = AbsPos_2_Position(Motor_Current_Steps[X_AXIS_INDEX]);
		    Motor_Current_Steps[Y_AXIS_INDEX] = AbsPos_2_Position(Motor_Current_Steps[Y_AXIS_INDEX]);
		    Motor_Current_Steps[Z_AXIS_INDEX] = AbsPos_2_Position(Motor_Current_Steps[Z_AXIS_INDEX]);
		    Motor_Current_Steps[M_AXIS_INDEX] = AbsPos_2_Position(Motor_Current_Steps[M_AXIS_INDEX]);

		if (encoder_count_x > 0)
		    {
		    htim2.Instance->CNT = 0;
		    speed_val = encoder_count_x;
		    speed_val = encoder_count_x*exp(encoder_count_x);

		    if(Motor_Current_Steps[X_AXIS_INDEX] > Motor_MAX_Steps[X_AXIS_INDEX])
			{
			speed_val = 0;
			}
		    run_flag = 1;
		    L6470_Run(X_AXIS_INDEX, L6470_DIR_FWD_ID, speed_val);
		    }
		else
		    {
		    htim2.Instance->CNT = 0;
		    encoder_count_x *= -1;
		    speed_val = encoder_count_x*exp(encoder_count_x);

		    if(Motor_Current_Steps[X_AXIS_INDEX] < 0)
			{
			speed_val = 0;
			}
		    run_flag = 1;
		    L6470_Run(X_AXIS_INDEX, L6470_DIR_REV_ID, speed_val);
		    }

		}
	    else if(run_flag)
		{
		run_flag = 0;
		L6470_Run(X_AXIS_INDEX, L6470_DIR_REV_ID, 0);
		}
	    }
	}
    }
