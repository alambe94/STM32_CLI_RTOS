/*
 * encoder_interface.c
 *
 *  Created on: 08-Mar-2019
 *      Author: medprime
 */
#include "encoder_interface.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;


extern int32_t Motor_Current_Steps[NO_OF_MOTORS]; // speed steps
extern int32_t Motor_MAX_Steps[NO_OF_MOTORS];
extern int32_t Motor_Parameter[NO_OF_MOTORS]; // speed steps
extern uint8_t Motor_Direction[NO_OF_MOTORS];
extern uint8_t Control_Mode;

void Encoder_Interface_Config()
    {
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
    }

void Encoder_Loop()
    {

    static uint32_t time_stamp = 0;
    static uint8_t runnig_flag_x = 0;
    static uint8_t runnig_flag_y = 0;
    static uint8_t runnig_flag_z = 0;

    int32_t steps_to_move = 0;
    int32_t speed_val = 0;

    uint8_t direction = L6470_DIR_FWD_ID;
    uint8_t perform_prepared_action = 0;
    char int_to_str[10] = {0};

    if (Control_Mode == 0)
	{

	int16_t encoder_count_x = htim2.Instance->CNT;
	int16_t encoder_count_y = htim1.Instance->CNT;
	int16_t encoder_count_z = htim4.Instance->CNT;
	int16_t encoder_count_m = encoder_count_x;

	if (HAL_GetTick() - time_stamp > 10)
	    {

	    time_stamp = HAL_GetTick();

	    L6470_PrepareGetParam(X_AXIS_INDEX, L6470_ABS_POS_ID);
	    L6470_PrepareGetParam(Y_AXIS_INDEX, L6470_ABS_POS_ID);
	    L6470_PrepareGetParam(Z_AXIS_INDEX, L6470_ABS_POS_ID);
	    L6470_PrepareGetParam(M_AXIS_INDEX, L6470_ABS_POS_ID);
	    L6470_PerformPreparedApplicationCommand();

	    Motor_Current_Steps[X_AXIS_INDEX] = L6470_ExtractReturnedData(
	    X_AXIS_INDEX, (uint8_t*) L6470_DaisyChainSpiRxStruct, 3);
	    Motor_Current_Steps[Y_AXIS_INDEX] = L6470_ExtractReturnedData(
	    Y_AXIS_INDEX, (uint8_t*) L6470_DaisyChainSpiRxStruct, 3);
	    Motor_Current_Steps[Z_AXIS_INDEX] = L6470_ExtractReturnedData(
	    Z_AXIS_INDEX, (uint8_t*) L6470_DaisyChainSpiRxStruct, 3);
	    Motor_Current_Steps[M_AXIS_INDEX] = L6470_ExtractReturnedData(
	    M_AXIS_INDEX, (uint8_t*) L6470_DaisyChainSpiRxStruct, 3);

	    Motor_Current_Steps[X_AXIS_INDEX] = AbsPos_2_Position(
		    Motor_Current_Steps[X_AXIS_INDEX]);
	    Motor_Current_Steps[Y_AXIS_INDEX] = AbsPos_2_Position(
		    Motor_Current_Steps[Y_AXIS_INDEX]);
	    Motor_Current_Steps[Z_AXIS_INDEX] = AbsPos_2_Position(
		    Motor_Current_Steps[Z_AXIS_INDEX]);
	    Motor_Current_Steps[M_AXIS_INDEX] = AbsPos_2_Position(
		    Motor_Current_Steps[M_AXIS_INDEX]);

	    if (encoder_count_x)
		{

		perform_prepared_action = 1;
		htim2.Instance->CNT = 0;

		if (encoder_count_x > 0)
		    {
		    direction = L6470_DIR_FWD_ID;
		    if(Motor_Current_Steps[X_AXIS_INDEX] > Motor_MAX_Steps[X_AXIS_INDEX] * 0.8)
			{
			encoder_count_x = encoder_count_x;
			if(Motor_Current_Steps[X_AXIS_INDEX] > Motor_MAX_Steps[X_AXIS_INDEX]*0.95)
			    {
			    encoder_count_x = 0;
			    }
			}
		    else
			{
			encoder_count_x = encoder_count_x* exp(encoder_count_x / 2);
			}
		    }
		else
		    {
		    encoder_count_x *= -1;
		    direction = L6470_DIR_REV_ID;
		    if(Motor_Current_Steps[X_AXIS_INDEX] < Motor_MAX_Steps[X_AXIS_INDEX] * 0.2)
			{
			encoder_count_x = encoder_count_x;
			if(Motor_Current_Steps[X_AXIS_INDEX] < Motor_MAX_Steps[X_AXIS_INDEX] * 0.05)
			    {
			    encoder_count_x = 0;
			    }
			}
		    else
			{
			encoder_count_x = encoder_count_x* exp(encoder_count_x / 2);
			}
		    }

		runnig_flag_x = 1;
		speed_val = Step_s_2_Speed(encoder_count_x);
		L6470_PrepareRun(X_AXIS_INDEX, direction, speed_val);

		}
	    else if(runnig_flag_x)
		{
		runnig_flag_x = 0;
		perform_prepared_action = 1;
		L6470_PrepareRun(X_AXIS_INDEX, direction, 0);
		}


	    if (perform_prepared_action)
		{
		perform_prepared_action = 0;
		L6470_PerformPreparedApplicationCommand();
		}


	    }
	}
    }
