/*
 * encoder_interface.c
 *
 *  Created on: 08-Mar-2019
 *      Author: medprime
 */
#include "encoder_interface.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


extern int32_t Motor_Current_Steps[NO_OF_MOTORS]; // speed steps
extern int32_t Motor_MAX_Steps[NO_OF_MOTORS];
extern int32_t Motor_Parameter[NO_OF_MOTORS]; // speed steps
extern uint8_t Motor_Direction[NO_OF_MOTORS];
extern uint8_t Control_Mode;


TIM_HandleTypeDef* Encoder_TIM[3] = {&htim1, &htim2, &htim3};


void Encoder_Interface_Config()
    {
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    }


uint8_t Encoder_Scan_Helper(uint8_t index)

    {

    static uint8_t runnig_flag[3] = {0};

    int16_t encoder_count[3] = {0};


    uint8_t direction = L6470_DIR_FWD_ID;
    int32_t speed_val = 0;

    uint8_t perform_prepared_action = 0;

    encoder_count[index] = Encoder_TIM[index]->Instance->CNT;

    if (encoder_count[index])
	{
	perform_prepared_action = 1;

	Encoder_TIM[index]->Instance->CNT = 0;

	if (encoder_count[index] > 0)
	    {
	    direction = L6470_DIR_FWD_ID;
	    if(Motor_Current_Steps[index] > Motor_MAX_Steps[index] * 0.8)
		{
		encoder_count[index] = encoder_count[index];
		if(Motor_Current_Steps[index] > Motor_MAX_Steps[index]*0.95)
		    {
		    encoder_count[index] = 0;
		    }
		}
	    else
		{
		encoder_count[index] = encoder_count[index]* exp(encoder_count[index] / 10);
		}
	    }
	else
	    {
	    encoder_count[index] *= -1;
	    direction = L6470_DIR_REV_ID;
	    if(Motor_Current_Steps[index] < Motor_MAX_Steps[index] * 0.2)
		{
		encoder_count[index] = encoder_count[index];
		if(Motor_Current_Steps[index] < Motor_MAX_Steps[index] * 0.05)
		    {
		    encoder_count[index] = 0;
		    }
		}
	    else
		{
		encoder_count[index] = encoder_count[index]* exp(encoder_count[index] / 10);
		}
	    }

	runnig_flag[index] = 1;
	speed_val = Step_s_2_Speed(encoder_count[index]);
	L6470_PrepareRun(index, direction, speed_val);

	}
    else if(runnig_flag[index])
	{
	runnig_flag[index] = 0;
	perform_prepared_action = 1;
	L6470_PrepareRun(index, direction, 0);
	}

    return perform_prepared_action;
    }

void Encoder_Loop()
    {

    static uint32_t time_stamp = 0;

    uint8_t perform_prepared_action = 0;

    if (Control_Mode == 0)
	{

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

	    perform_prepared_action  = Encoder_Scan_Helper(X_AXIS_INDEX);
	    perform_prepared_action |= Encoder_Scan_Helper(Y_AXIS_INDEX);
	    perform_prepared_action |= Encoder_Scan_Helper(Z_AXIS_INDEX);

	    if (perform_prepared_action)
		{
		perform_prepared_action = 0;
		L6470_PerformPreparedApplicationCommand();
		}


	    }
	}
    }
