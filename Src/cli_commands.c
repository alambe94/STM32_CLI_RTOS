/*
 * CLI_commands.c
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#include "cli_commands.h"

extern int32_t Motor_Current_Steps[NO_OF_MOTORS];
extern int32_t Motor_MAX_Steps[NO_OF_MOTORS];
extern int32_t Motor_Parameter[NO_OF_MOTORS]; // speed steps
extern uint8_t Motor_Direction[NO_OF_MOTORS];
extern uint8_t Control_Mode;


static uint8_t Cammand_Executing_Flag = 0;

#define AT24CXX_ADDRESS    0XA6
#define AT24CXX_PAGE_SIZE 8 //page size


/*************************************************************************/

void Move_Callback_Helper(uint8_t axis_index)
    {

    if (Motor_Direction[axis_index])
	{

	if (Motor_Current_Steps[axis_index] + Motor_Parameter[axis_index]
		> Motor_MAX_Steps[axis_index])
	    {
	    Motor_Parameter[axis_index] = Motor_MAX_Steps[axis_index] - Motor_Current_Steps[axis_index];
	    }
	}
    else
	{
	Motor_Parameter[axis_index] = 0;
	}

	L6470_PrepareMove(axis_index, Motor_Direction[axis_index],
		Motor_Parameter[axis_index]);
    }


void Goto_Callback_Helper(uint8_t axis_index)
    {

    uint8_t direction = L6470_DIR_FWD_ID;

    if (Motor_Direction[axis_index])
	{
	if (Motor_Parameter[axis_index] > Motor_MAX_Steps[axis_index])
	    {
	    Motor_Parameter[axis_index] = Motor_MAX_Steps[axis_index];
	    }

	if (Motor_Parameter[axis_index] < Motor_Current_Steps[axis_index])
	    {
	    direction = L6470_DIR_REV_ID; //reverse
	    }
	}
    else
	{
	Motor_Parameter[axis_index] = 0;
	direction = L6470_DIR_REV_ID;
	}

    L6470_PrepareGoToDir(axis_index, direction,
	    Motor_Parameter[axis_index]);
    }


void Run_Callback_Helper(uint8_t axis_index)
    {

    uint32_t speed = 0;

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


void Speed_Callback_Helper(uint8_t axis_index)
    {

    uint32_t speed = 0;

    if (Motor_Parameter[axis_index] > 15000)
	{
	Motor_Parameter[axis_index] = 15000;
	}

    speed = Step_s_2_Speed(Motor_Parameter[axis_index]);
    L6470_PrepareSetParam(axis_index, L6470_MAX_SPEED_ID, speed);
    }



uint8_t Store_In_AT24(uint8_t location, int32_t data)
    {
        uint8_t i2c_frame[AT24CXX_PAGE_SIZE] = {0};
        uint8_t i2c_status = 0;
	i2c_frame[0] = location; //loaction in at24c

	itoa(data,(char*)&i2c_frame[1],10);

	if(HAL_I2C_Master_Transmit(&hi2c1, AT24CXX_ADDRESS, i2c_frame, AT24CXX_PAGE_SIZE+1 ,5) == HAL_OK)
	    {
	    i2c_status = 1;
	    }
	return i2c_status;
    }

uint8_t Get_From_AT24(uint8_t location, int32_t* data)
    {
    uint8_t i2c_frame[AT24CXX_PAGE_SIZE] = {0};
    uint8_t i2c_status = 0;
    // set read pointer in at24c //dummy write
    if (HAL_I2C_Master_Transmit(&hi2c1, AT24CXX_ADDRESS, &location, 1, 5)
	    == HAL_OK)
	{
	if (HAL_I2C_Master_Receive(&hi2c1, AT24CXX_ADDRESS, i2c_frame,
		AT24CXX_PAGE_SIZE, 5) == HAL_OK)
	    {
	    i2c_status = 1;
	    *data = atoi((char*)i2c_frame);
	    }
	}

    return i2c_status;
    }


void Print_Command_Ok(char* cli_tx_out_buffer, char* cli_rx_command)
    {
    strncat(cli_tx_out_buffer, "\r\nOk->", 20);
    strncat(cli_tx_out_buffer, cli_rx_command, strlen(cli_rx_command));
    strncat(cli_tx_out_buffer, "\r\n", 3);
    }

void Print_Command_Err(char* cli_tx_out_buffer, char* cli_rx_command)
    {
    strncat(cli_tx_out_buffer, "\r\nInvalid Command:", 50);
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


uint8_t Parse_Int(char* param, int32_t* param_int)
    {

    char str_to_int[11] = "0";
    uint8_t i = 0;
    uint8_t ok_flag = 1;

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
	*param_int = atoi(str_to_int);
	}

    return ok_flag;
    }


uint8_t Move_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint8_t  is_command_valid = 1;
    uint8_t  param_number = 1;//param number, starting from first
    uint8_t  param_counts = 0;//received param count
    uint16_t param_len = 0;
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

	param_ptr = CLI_Get_Parameter(cli_rx_command,
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
		    Move_Callback_Helper(X_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'y':
		if (Parse_Parameters(Y_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Move_Callback_Helper(Y_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'z':
		if (Parse_Parameters(Z_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Move_Callback_Helper(Z_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'm':
		if (Parse_Parameters(M_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Move_Callback_Helper(M_AXIS_INDEX);
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
	Cammand_Executing_Flag = 1;
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
    .CLI_Command_Description = "\r\n move: move stepper for given steps\r\n",
    .CLI_Callback = Move_Callback, /* The function to run. */
    };




uint8_t Goto_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint8_t  is_command_valid = 1;
    uint8_t  param_number = 1;//param number, starting from first
    uint8_t  param_counts = 0;//received param count
    uint16_t param_len = 0;
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

	param_ptr = CLI_Get_Parameter(cli_rx_command,
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
		    Goto_Callback_Helper(X_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'y':
		if (Parse_Parameters(Y_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Goto_Callback_Helper(Y_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'z':
		if (Parse_Parameters(Z_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Goto_Callback_Helper(Z_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'm':
		if (Parse_Parameters(M_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Goto_Callback_Helper(M_AXIS_INDEX);
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
	Cammand_Executing_Flag = 1;
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

	}

    return 0;// operation complete do not call again

    }


CLI_Command_t Goto_Defination =
    {
    .CLI_Command = "goto", /* The command string to type. */
    .CLI_Command_Description = "\r\n goto: move motor to absolute position\r\n",
    .CLI_Callback = Goto_Callback, /* The function to run. */
    };




uint8_t Speed_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint8_t  is_command_valid = 1;
    uint16_t param_len = 0;
    uint8_t  param_number = 1;//param number, starting from first
    uint8_t  param_counts = 0;//received param count
    char*    param_ptr = NULL;

    if (Control_Mode)
	{

	do
	    {

	    param_ptr = CLI_Get_Parameter(cli_rx_command, param_number++,
		    &param_len);

	    if (param_ptr != NULL)
		{

		param_counts++;

		switch (*param_ptr)
		    {
		case 'x':
		    if (Parse_Parameters(X_AXIS_INDEX, (param_ptr + 1),
			    param_len))
			{
			Speed_Callback_Helper(X_AXIS_INDEX);
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
			Speed_Callback_Helper(Y_AXIS_INDEX);
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
			Speed_Callback_Helper(Z_AXIS_INDEX);
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
			Speed_Callback_Helper(M_AXIS_INDEX);
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
	    Cammand_Executing_Flag = 1;
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
    .CLI_Command_Description = "\r\n speed: set max speed of a motor\r\n",
    .CLI_Callback = Speed_Callback, /* The function to run. */
    };




uint8_t Run_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
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

	param_ptr = CLI_Get_Parameter(cli_rx_command,
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
		    Run_Callback_Helper(X_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'y':
		if (Parse_Parameters(Y_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Run_Callback_Helper(Y_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'z':
		if (Parse_Parameters(Z_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Run_Callback_Helper(Z_AXIS_INDEX);
		    }
		else
		    {
		    is_command_valid = 0;
		    }
		break;

	    case 'm':
		if (Parse_Parameters(M_AXIS_INDEX, (param_ptr+1), param_len))
		    {
		    Run_Callback_Helper(M_AXIS_INDEX);
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
	Cammand_Executing_Flag = 1;
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
    .CLI_Command_Description = "\r\n run: run motor at given speed\r\n",
    .CLI_Callback = Run_Callback, /* The function to run. */
    };




uint8_t Home_Callback(char* cli_rx_command, char* cli_tx_out_buffer)

    {

    uint8_t  is_command_valid = 1;
    uint8_t  param_number = 1;//param number, starting from first
    uint8_t  param_counts = 0;//received param count
    uint16_t param_len = 0;
    char*    param_ptr = NULL;

    uint32_t speed = Step_s_2_Speed(6000);

    do
	{

	param_ptr = CLI_Get_Parameter(cli_rx_command,
		                           param_number++,
		                           &param_len);
	if (param_ptr != NULL)
	    {

	    if (param_len == 1)
		{

	    param_counts++;

	    switch (*param_ptr)
		{
	    case 'x':
		    L6470_PrepareGoUntil(X_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		break;

	    case 'y':
		    L6470_PrepareGoUntil(Y_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		break;

	    case 'z':
		    L6470_PrepareGoUntil(Z_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		break;

	    case 'm':
		    L6470_PrepareGoUntil(M_AXIS_INDEX,
			    L6470_ACT_RST_ID,
			    L6470_DIR_REV_ID,
			    speed);
		break;

	    default:
		    is_command_valid = 0;
		break;

		}
	    }
	else
	    {
	    is_command_valid = 0;
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
	Cammand_Executing_Flag = 1;
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
    .CLI_Command_Description = "\r\n home: goto home position\r\n",
    .CLI_Callback = Home_Callback, /* The function to run. */
    };



uint8_t Auto_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint16_t param_len = 0;

    CLI_Get_Parameter(cli_rx_command,1,&param_len);

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
    .CLI_Command_Description = "\r\n auto: control from android\r\n",
    .CLI_Callback = Auto_Callback, /* The function to run. */
    };




uint8_t Manual_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint16_t param_len = 0;

    CLI_Get_Parameter(cli_rx_command,1,&param_len);

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
    .CLI_Command_Description = "\r\n manual: control from joystick\r\n",
    .CLI_Callback = Manual_Callback, /* The function to run. */
    };




uint8_t Getpos_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint16_t param_len = 0;

    CLI_Get_Parameter(cli_rx_command,1,&param_len);

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

	    sprintf(cli_tx_out_buffer + strlen(cli_tx_out_buffer),"pos x%i y%i z%i m%i\r\nDone\r\n",(int)Motor_Current_Steps[X_AXIS_INDEX],(int)Motor_Current_Steps[Y_AXIS_INDEX],
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
    .CLI_Command_Description = "\r\n getpos: return current position of motors\r\n",
    .CLI_Callback = Getpos_Callback, /* The function to run. */
    };




/* expecting 4 params*/
uint8_t Setfocus_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint8_t is_command_valid = 1;
    uint8_t is_at24_write_ok = 0;
    uint16_t param_len = 0;
    int32_t lf = 0;
    int32_t lfmin = 0;
    int32_t lfmax = 0;
    uint8_t store_index = 0;
    char* param_ptr = NULL;

    // if params are more than 4, command is invalid
    // check for fifth param
    param_ptr = CLI_Get_Parameter(cli_rx_command, 5, &param_len);

    if (param_ptr == NULL)
	{
	//get first param
	param_ptr = CLI_Get_Parameter(cli_rx_command, 1, &param_len);

	if(param_len == 2)
	    {
	    if(strncmp(param_ptr,"l1",2) == 0)
		{
		store_index = 0;
		}
	    else if(strncmp(param_ptr,"l2",2) == 0)
		{
		store_index = 3;
		}
	    else if(strncmp(param_ptr,"l3",2) == 0)
		{
		store_index = 6;
		}
	    else if(strncmp(param_ptr,"l4",2) == 0)
		{
		store_index = 9;
		}
	    else
		{
		is_command_valid = 0;
		}
	    }
	else
	    {
	    is_command_valid = 0;
	    }
	}
    else
	{
	is_command_valid = 0;
	}


    //get second param
    param_ptr = CLI_Get_Parameter(cli_rx_command, 2, &param_len);
    if (!Parse_Int(param_ptr, &lf))
	{
	is_command_valid = 0;
	}
    //get third param
    param_ptr = CLI_Get_Parameter(cli_rx_command, 3, &param_len);
    if (!Parse_Int(param_ptr, &lfmin))
	{
	is_command_valid = 0;
	}
    //get forth param
    param_ptr = CLI_Get_Parameter(cli_rx_command, 4, &param_len);
    if(param_ptr != NULL)
	{
    if (!Parse_Int(param_ptr, &lfmax))
	{
	is_command_valid = 0;
	}
	}
    else
	{
	is_command_valid = 0;
	}


    if(is_command_valid)
	{
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);
	is_at24_write_ok = Store_In_AT24((store_index+0)*AT24CXX_PAGE_SIZE,lf);
	HAL_Delay(5);
	is_at24_write_ok += Store_In_AT24((store_index+1)*AT24CXX_PAGE_SIZE,lfmin);
	HAL_Delay(5);
	is_at24_write_ok += Store_In_AT24((store_index+2)*AT24CXX_PAGE_SIZE,lfmax);

	    if(is_at24_write_ok == 3)
		{
		strncat(cli_tx_out_buffer,"\r\nDone\r\n",20);
		}
	    else
		{
		strncat(cli_tx_out_buffer, "AT24C Write Failed\r\n", 50);
		}
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}


    return 0;
    }


CLI_Command_t Setfocus_Defination =
    {
    .CLI_Command = "setfocus", /* The command string to type. */
    .CLI_Command_Description = "\r\n setfocus: store lens position in EEPROM\r\n",
    .CLI_Callback = Setfocus_Callback, /* The function to run. */
    };



uint8_t Getfocus_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    uint8_t  is_command_valid = 1;
    uint8_t  is_at24_read_ok = 0;
    int32_t  lf = 0;
    int32_t  lfmin = 0;
    int32_t  lfmax = 0;
    uint8_t  store_index = 0;
    uint8_t  lense_no = 0;

    uint16_t param_len = 0;
    char*    param_ptr = NULL;

    // if params are more than one, command is invalid
    // check for second param
    param_ptr = CLI_Get_Parameter(cli_rx_command, 2, &param_len);

    if (param_ptr == NULL)
	{
	//get first param
	param_ptr = CLI_Get_Parameter(cli_rx_command, 1, &param_len);

	if(param_len == 2)
	    {
	    if(strncmp(param_ptr,"l1",2) == 0)
		{
		store_index = 0;
		lense_no = 1;
		}
	    else if(strncmp(param_ptr,"l2",2) == 0)
		{
		store_index = 3;
		lense_no = 2;
		}
	    else if(strncmp(param_ptr,"l3",2) == 0)
		{
		store_index = 6;
		lense_no = 3;
		}
	    else if(strncmp(param_ptr,"l4",2) == 0)
		{
		store_index = 9;
		lense_no = 4;
		}
	    else
		{
		is_command_valid = 0;
		}
	    }
	else
	    {
	    is_command_valid = 0;
	    }
	}
    else
	{
	is_command_valid = 0;
	}



    if (is_command_valid)
	{
	Print_Command_Ok(cli_tx_out_buffer, cli_rx_command);

	is_at24_read_ok  = Get_From_AT24((store_index+0)*AT24CXX_PAGE_SIZE, &lf);
	is_at24_read_ok += Get_From_AT24((store_index+1)*AT24CXX_PAGE_SIZE, &lfmin);
	is_at24_read_ok += Get_From_AT24((store_index+2)*AT24CXX_PAGE_SIZE, &lfmax);

	if(is_at24_read_ok == 3)
	    {
	    sprintf(cli_tx_out_buffer + strlen(cli_tx_out_buffer), "l%i %i %i %i\r\nDone\r\n",lense_no,(int)lf,(int)lfmin,(int)lfmax);
	    }
	else
	    {
	    strncat(cli_tx_out_buffer, "AT24C Read Failed\r\n", 50);
	    }
	}
    else
	{
	Print_Command_Err(cli_tx_out_buffer, cli_rx_command);
	}

    return 0;// operation complete do not call again

    }


CLI_Command_t Getfocus_Defination =
    {
    .CLI_Command = "getfocus", /* The command string to type. */
    .CLI_Command_Description = "\r\n getfocus: retrieve lens position from EEPROM\r\n",
    .CLI_Callback = Getfocus_Callback, /* The function to run. */
    };







uint8_t Setlense_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    return 0;// operation complete do not call again

    }


CLI_Command_t Setlense_Defination =
    {
    .CLI_Command = "setlense", /* The command string to type. */
    .CLI_Command_Description = "\r\n setlense: lense change\r\n",
    .CLI_Callback = Setlense_Callback, /* The function to run. */
    };




uint8_t Getlense_Callback(char* cli_rx_command, char* cli_tx_out_buffer)
    {

    return 0;// operation complete do not call again

    }


CLI_Command_t Getlense_Defination =
    {
    .CLI_Command = "getlense", /* The command string to type. */
    .CLI_Command_Description = "\r\n getlense: get current lense\r\n",
    .CLI_Callback = Getlense_Callback, /* The function to run. */
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
    CLI_Add_Cammand(&Goto_Defination);
    CLI_Add_Cammand(&Setfocus_Defination);
    CLI_Add_Cammand(&Getfocus_Defination);
    CLI_Add_Cammand(&Setlense_Defination);
    CLI_Add_Cammand(&Getlense_Defination);

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



    if (GPIO_Pin == L6470_BUSY_SYNC_INT_Pin)
	{
	if (Cammand_Executing_Flag)
	{
	Cammand_Executing_Flag = 0;
	CLI_UART_Send_String("\r\nDone\r\n\r\n->");
	}
	}

    }



