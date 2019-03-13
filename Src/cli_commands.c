/*
 * CLI_commands.c
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#include "cli_commands.h"

extern int32_t Motor_Current_Steps[NO_OF_MOTORS]; // speed steps
extern int32_t Motor_MAX_Steps[NO_OF_MOTORS];
extern int32_t Motor_Parameter[NO_OF_MOTORS]; // speed steps
extern uint8_t Motor_Direction[NO_OF_MOTORS];
extern uint8_t Control_Mode;


/*************************************************************************/

void Prepare_Move_Cammand(uint8_t axis_index)
    {

    if (Motor_Direction[axis_index])
	{

	if (Motor_Current_Steps[axis_index] + Motor_Parameter[axis_index]
		> Motor_MAX_Steps[axis_index])
	    {
	    Motor_Parameter[axis_index] = Motor_MAX_Steps[axis_index] - Motor_Current_Steps[axis_index];
	    }
	}

	L6470_PrepareMove(axis_index, Motor_Direction[axis_index],
		Motor_Parameter[axis_index]);
    }


void Prepare_Run_Cammand(uint8_t axis_index)
    {

    uint16_t speed = 0;

    if (Motor_Current_Steps[axis_index] > Motor_MAX_Steps[axis_index])
	{
	//Motor_Parameter[axis_index] = 0;
	}
    if (Motor_Current_Steps[axis_index] < 0)
	{
	//Motor_Parameter[axis_index] = 0;
	}
    speed = Step_s_2_Speed(Motor_Parameter[axis_index]);
    L6470_PrepareRun(axis_index, Motor_Direction[axis_index], speed);
    }


void Prepare_Speed_Cammand(uint8_t axis_index)
    {

    uint16_t speed = 0;

    if (Motor_Parameter[axis_index] > 15000)
	{
	Motor_Parameter[axis_index] = 15000;
	}

    speed = Step_s_2_Speed(Motor_Parameter[axis_index]);
    L6470_PrepareSetParam(axis_index, L6470_MAX_SPEED_ID, speed);
    }


void Print_Command_Ok(char* cli_tx_out_buffer, char* cli_rx_command)
    {
    strncat(cli_tx_out_buffer, "\r\nOk->", 50);
    strncat(cli_tx_out_buffer, cli_rx_command, strlen(cli_rx_command));
    strncat(cli_tx_out_buffer, "\r\n", 3);
    }

void Print_Command_Err(char* cli_tx_out_buffer, char* cli_rx_command)
    {
    strncat(cli_tx_out_buffer, "\r\nInvalid Command->", 50);
    strncat(cli_tx_out_buffer, cli_rx_command, strlen(cli_rx_command));
    strncat(cli_tx_out_buffer, "\r\n", 3);
    }


uint8_t Parse_Parameters(uint8_t index, char* param, uint16_t param_len)
    {

    char str_to_int[11] = "0";
    uint8_t i = 0;
    uint8_t ok_flag = 1;

    if (param_len == 1)
	{
	 return 0;
	}

    if (*param == '-')
	{
	param++;
	Motor_Direction[index] = L6470_DIR_REV_ID; //reverse , toward home
	}
    else
	{
	Motor_Direction[index] = L6470_DIR_FWD_ID; //forward away from home
	}

    while (*param != '\0' && *param != ' ')
	{

	if (*param < '0' || *param > '9' || i >= 10)
	    {
	    ok_flag = 0; //not number
	    break;
	    }
	str_to_int[i++] = *param++;
	}

    if(ok_flag)
	{


	Motor_Parameter[index] = atoi(str_to_int);
	}

    return ok_flag;
    }



uint8_t Move_Callback(char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t cmd_len)
    {

    uint8_t  is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t  param_number = 1;//param number, starting from first
    uint8_t  param_counts = 0;//received param count
    char*    param_ptr = NULL;

    if(Control_Mode)
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


    do
	{

	param_ptr = CLI_UART_Get_Parameter(cli_rx_command,
		                           param_number++,
		                           &param_len);

	if (param_ptr != NULL)
	    {

	    param_counts++;

	    switch (*param_ptr)
		{
	    case 'x':
		if (Parse_Parameters(X_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Move_Cammand(X_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'y':
		if (Parse_Parameters(Y_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Move_Cammand(Y_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'z':
		if (Parse_Parameters(Z_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Move_Cammand(Z_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'm':
		if (Parse_Parameters(M_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Move_Cammand(M_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;
	    default:
		is_command_valid = 0;
		break;

		}
	    }

	}
    while (param_ptr != NULL);

    if (!param_counts)// no param received
	{
	is_command_valid = 0;
	}

    if (is_command_valid)
	{
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	L6470_PerformPreparedApplicationCommand();
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

	}

    return 0;// operation complete do not call again

    }


CLI_Command_t Move_Defination =
    {
    .CLI_Command = "move", /* The command string to type. */
    .CLI_Command_Description = "\r\n move: Move the motor for given steps\r\n",
    .CLI_Callback = Move_Callback, /* The function to run. */
    };




uint8_t Speed_Callback(char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t cmd_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 1;
    char* param_ptr = NULL;

    if (Control_Mode)
	{

	do
	    {

	    param_ptr = CLI_UART_Get_Parameter(cli_rx_command, param_number++,
		    &param_len);

	    if (param_ptr != NULL)
		{

		switch (*param_ptr)
		    {
		case 'x':
		    if (Parse_Parameters(X_AXIS_INDEX, (param_ptr + 1),
			    param_len))
			{
			Prepare_Speed_Cammand(X_AXIS_INDEX);
			}
		    else
			{
			is_command_valid = 0;
			}
		    break;

		case 'y':
		    if (Parse_Parameters(Y_AXIS_INDEX, (param_ptr + 1),
			    param_len))
			{
			Prepare_Speed_Cammand(Y_AXIS_INDEX);
			}
		    else
			{
			is_command_valid = 0;
			}
		    break;

		case 'z':
		    if (Parse_Parameters(Z_AXIS_INDEX, (param_ptr + 1),
			    param_len))
			{
			Prepare_Speed_Cammand(Z_AXIS_INDEX);
			}
		    else
			{
			is_command_valid = 0;
			}
		    break;

		case 'm':
		    if (Parse_Parameters(M_AXIS_INDEX, (param_ptr + 1),
			    param_len))
			{
			Prepare_Speed_Cammand(M_AXIS_INDEX);
			}
		    else
			{
			is_command_valid = 0;
			}
		    break;
		default:
		    is_command_valid = 0;
		    break;

		    }
		}

	    }
	while (param_ptr != NULL);

	if (is_command_valid)
	    {
	    Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	    L6470_PerformPreparedApplicationCommand();
	    }
	else
	    {
	    Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	    }

	}

    return 0; // operation complete do not call again
    }


CLI_Command_t Speed_Defination =
    {
    .CLI_Command = "speed", /* The command string to type. */
    .CLI_Command_Description = "\r\n speed: Set motor speed steps/sec\r\n",
    .CLI_Callback = Speed_Callback, /* The function to run. */
    };




uint8_t Run_Callback(char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t cmd_len)
    {

    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    char* param_ptr = NULL;
    uint8_t param_number = 1;

    if(Control_Mode)
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


    do
	{

	param_ptr = CLI_UART_Get_Parameter(cli_rx_command,
		                           param_number++,
		                           &param_len);

	if (param_ptr != NULL)
	    {

	    switch (*param_ptr)
		{
	    case 'x':
		if (Parse_Parameters(X_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Run_Cammand(X_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'y':
		if (Parse_Parameters(Y_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Run_Cammand(Y_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'z':
		if (Parse_Parameters(Z_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Run_Cammand(Z_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'm':
		if (Parse_Parameters(M_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Prepare_Run_Cammand(M_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;
	    default:
		is_command_valid = 0;
		break;

		}
	    }

	}
    while (param_ptr != NULL);

    if (is_command_valid)
	{
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	L6470_PerformPreparedApplicationCommand();
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}
	}

    return 0;// operation complete do not call again

    }


CLI_Command_t Run_Defination =
    {
    .CLI_Command = "run", /* The command string to type. */
    .CLI_Command_Description = "\r\n run: Run motor at given speed\r\n",
    .CLI_Callback = Run_Callback, /* The function to run. */
    };




uint8_t Home_Callback(char* cli_rx_command, char* cli_tx_out_buffer, uint16_t cmd_len)

    {
    uint8_t is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t param_number = 1;

    char* param_ptr = NULL;

    uint16_t speed = Step_s_2_Speed(6000);

    do
	{

	param_ptr = CLI_UART_Get_Parameter(cli_rx_command,
		                           param_number++,
		                           &param_len);

	if (param_ptr != NULL)
	    {

	    switch (*param_ptr)
		{
	    case 'x':
		if(param_len == 1)
		    {
		    L6470_PrepareGoUntil(X_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'y':
		if(param_len == 1)
		    {
		    L6470_PrepareGoUntil(Y_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'z':
		if(param_len == 1)
		    {
		    L6470_PrepareGoUntil(Z_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'm':
		if(param_len == 1)
		    {
		    L6470_PrepareGoUntil(M_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    default:
		    is_command_valid = 0;
		break;

		}
	    }

	}
    while (param_ptr != NULL);

    if (is_command_valid)
	{
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	L6470_PerformPreparedApplicationCommand();
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;// operation complete do not call again

    }


CLI_Command_t Home_Defination =
    {
    .CLI_Command = "home", /* The command string to type. */
    .CLI_Command_Description = "\r\n home: Run given axis to home position\r\n",
    .CLI_Callback = Home_Callback, /* The function to run. */
    };



uint8_t Auto_Callback(char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t cmd_len)
    {

    uint16_t param_len = 0;

    CLI_UART_Get_Parameter(cli_rx_command,1,&param_len);

    if(param_len == 0)
	{
	Control_Mode = 1;
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

    return 0; // operation complete do not call again

    }


CLI_Command_t Auto_Defination =
    {
    .CLI_Command = "auto", /* The command string to type. */
    .CLI_Command_Description = "\r\n auto: Control from android\r\n",
    .CLI_Callback = Auto_Callback, /* The function to run. */
    };




uint8_t Manual_Callback(char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t cmd_len)
    {

    uint16_t param_len = 0;

    CLI_UART_Get_Parameter(cli_rx_command,1,&param_len);

    if(param_len == 0)
	{
	Control_Mode = 0;
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

    return 0; // operation complete do not call again

    }


CLI_Command_t Manual_Defination =
    {
    .CLI_Command = "manual", /* The command string to type. */
    .CLI_Command_Description = "\r\n manual: Control from joystick\r\n",
    .CLI_Callback = Manual_Callback, /* The function to run. */
    };




uint8_t Getpos_Callback(char* cli_rx_command, char* cli_tx_out_buffer,
	uint16_t cmd_len)
    {

    uint16_t param_len = 0;

    CLI_UART_Get_Parameter(cli_rx_command,1,&param_len);

    if(param_len == 0)
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

	    Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);

	    sprintf(cli_tx_out_buffer + strlen(cli_tx_out_buffer),"x%i y%i z%i m%i\r\n",(int)Motor_Current_Steps[X_AXIS_INDEX],(int)Motor_Current_Steps[Y_AXIS_INDEX],
		    (int)Motor_Current_Steps[Z_AXIS_INDEX],(int)Motor_Current_Steps[M_AXIS_INDEX]);

	}
    else
	{
	    Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

    return 0; // operation complete do not call again

    }


CLI_Command_t Getpos_Defination =
    {
    .CLI_Command = "getpos", /* The command string to type. */
    .CLI_Command_Description = "\r\n getpos: Return current position of motors\r\n",
    .CLI_Callback = Getpos_Callback, /* The function to run. */
    };


/*************************************************************************/



void CLI_Add_All_Commands()
    {

    CLI_Add_Cammand(&Move_Defination);
    CLI_Add_Cammand(&Speed_Defination);
    CLI_Add_Cammand(&Run_Defination);
    CLI_Add_Cammand(&Home_Defination);
    CLI_Add_Cammand(&Auto_Defination);
    CLI_Add_Cammand(&Manual_Defination);
    CLI_Add_Cammand(&Getpos_Defination);


    }

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {

    if (GPIO_Pin == L6470_Flag_INT_Pin)
	{

	uint16_t status_register_x = L6470_GetStatus(X_AXIS_INDEX);
	uint16_t status_register_y = L6470_GetStatus(Y_AXIS_INDEX);
	uint16_t status_register_z = L6470_GetStatus(Z_AXIS_INDEX);
	uint16_t status_register_m = L6470_GetStatus(M_AXIS_INDEX);

	if ((status_register_x & STATUS_STEP_LOSS_A) == 0
		|| (status_register_x & STATUS_STEP_LOSS_B) == 0)
	    {
	    L6470_HardStop(X_AXIS_INDEX);
	    }
	if ((status_register_y & STATUS_STEP_LOSS_A) == 0
		|| (status_register_y & STATUS_STEP_LOSS_B) == 0)
	    {
	    L6470_HardStop(Y_AXIS_INDEX);
	    }
	if ((status_register_z & STATUS_STEP_LOSS_A) == 0
		|| (status_register_z & STATUS_STEP_LOSS_B) == 0)
	    {
	    L6470_HardStop(Z_AXIS_INDEX);
	    }
	if ((status_register_m & STATUS_STEP_LOSS_A) == 0
		|| (status_register_m & STATUS_STEP_LOSS_B) == 0)
	    {
	    L6470_HardStop(M_AXIS_INDEX);
	    }

	if (status_register_x & STATUS_SW_EVN)
	    {
	    L6470_HardStop(X_AXIS_INDEX);
	    L6470_ReleaseSW(X_AXIS_INDEX, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	    }
	if (status_register_y & STATUS_SW_EVN)
	    {
	    L6470_HardStop(Y_AXIS_INDEX);
	    L6470_ReleaseSW(Y_AXIS_INDEX, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	    }
	if (status_register_z & STATUS_SW_EVN)
	    {
	    L6470_HardStop(Z_AXIS_INDEX);
	    L6470_ReleaseSW(Z_AXIS_INDEX, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	    }
	if (status_register_m & STATUS_SW_EVN)
	    {
	    L6470_ReleaseSW(M_AXIS_INDEX, L6470_ACT_RST_ID, L6470_DIR_FWD_ID);
	    }
	}
    }



