/*
 * CLI_commands.c
 *
 *  Created on: 19-Oct-2018
 *      Author: medprime
 */

#include "CLI_commands.h"



#define NO_OF_MOTORS 4
char motor_name[NO_OF_MOTORS] = {'x','y','z','m'};
char motor_name_caps[NO_OF_MOTORS] = {'X','Y','Z','M'};
char millis_int_to_str[10] = {0};
extern uint32_t   millis1;
extern UART_HandleTypeDef huart2;


uint8_t str_to_int(const char *str, uint32_t* steps, uint8_t* direction) //return if success
    {
    const char* str_copy = 0;
    uint8_t xreturn = 1;
    str_copy = str;

    if(*str_copy == '-')
	{
	str_copy++;
	str++;//skip sign
	*direction = 1; //reverse direction
	}
    if(*str_copy  == '\0' || *str_copy == ' ')//first char is null or space
	{
	xreturn = 0;//not number
	}
    while(*str_copy  != '\0' && *str_copy != ' ')
	{
	if(*str_copy < '0' || *str_copy > '9')
	    {
	    xreturn = 0;//not number
	    }
	str_copy++;
	}
    if(xreturn)
	{
	*steps = atoi(str);
	}
    return xreturn;
    }


/*************************************************************************/
 BaseType_t G28_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "G28 OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t G28_defination =
    {
    "G28", /* The command string to type. */
    "\r\nG28: Motor Home\r\n",
    &G28_callback, /* The function to run. */
    -1 /* The user can enter any number of parameter. */
    };
/*************************************************************************/


/*************************************************************************/

BaseType_t G1_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "G1 OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t G1_defination =
    {
    "G1", /* The command string to type. */
    "\r\nG1: Motor Move\r\n",
    &G1_callback, /* The function to run. */
    -1 /* The user can enter any number of parameter. */
    };
/*************************************************************************/



/*************************************************************************/

BaseType_t G0_callback(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
    {

    const char* invalid_parameter = "Invalid parameter ";
    const char* pcParameter;
    BaseType_t xParameterStringLength;
    UBaseType_t uxParameterNumber = 1;
    uint32_t motor_steps[NO_OF_MOTORS] = {0};
    uint8_t  motor_direction[NO_OF_MOTORS] = {0};
    uint8_t is_command_valid = 1;  //reset if any param is invalid
    uint8_t param_received = 0; //set if at least one param is valid

    /* Remove compile time warnings about unused parameters, and check the
     write buffer is not NULL.  NOTE - for simplicity, this example assumes the
     write buffer length is adequate, so does not check for buffer overflows. */
    (void) pcCommandString;
    (void) xWriteBufferLen;

    // to make sure pcWriteBuffer is always null terminated.
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);

    sprintf(pcWriteBuffer, "G0 Ok:\r\n");

    do
	{
	/* Obtain the parameter string. */
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, /* The command string itself. */
	uxParameterNumber, /* Return the next parameter. */
	&xParameterStringLength /* Store the parameter string length. */
	);

	if (pcParameter != NULL)
	    {

	    uxParameterNumber++;

	    switch (*pcParameter)
		{

	    case 'x':
	    case 'X':
		{
		if (str_to_int((pcParameter + 1), &motor_steps[0], &motor_direction[0]) == 1)
		    {
		    // string to int success
		    param_received = 1;
		    strncat(pcWriteBuffer, "X=",2);
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for x
		    is_command_valid = 0;
		    strncat(pcWriteBuffer, invalid_parameter, strlen(invalid_parameter));
		    strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }

		}
		break;

	    case 'y':
	    case 'Y':
		{
		if (str_to_int((pcParameter + 1), &motor_steps[1], &motor_direction[1]) == 1)
		    {
		    // string to int success
		    param_received = 1;
		    strncat(pcWriteBuffer, "Y=",2);
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for y
		    is_command_valid = 0;
		    strncat(pcWriteBuffer, invalid_parameter,3);
		    strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		}
		break;

	    case 'z':
	    case 'Z':
		{
		if (str_to_int((pcParameter + 1), &motor_steps[2], &motor_direction[2]) == 1)
		    {
		    // success
		    param_received = 1;
		    strncat(pcWriteBuffer, "Z=",2);
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for z
		    is_command_valid = 0;
		    strncat(pcWriteBuffer, invalid_parameter,strlen(invalid_parameter));
		    strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		}
		break;

	    case 'm':
	    case 'M':
		{
		if (str_to_int((pcParameter + 1), &motor_steps[3], &motor_direction[0]) == 1)
		    {
		    // success
		    param_received = 1;
		    strncat(pcWriteBuffer, "M=",2);
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for m
		    is_command_valid = 0;
		    strncat(pcWriteBuffer, invalid_parameter,strlen(invalid_parameter));
		    strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		}
		break;

	    default:
		{
		//invalid parameter(s)
		param_received = 1; //received but invalid
		is_command_valid = 0;
		strncat(pcWriteBuffer, invalid_parameter,strlen(invalid_parameter));
		strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		strncat(pcWriteBuffer, "\r\n", strlen("\r\n"));
		}

		}
	    }
	else if(param_received == 0 && is_command_valid == 1) //zero param entered
	    {
		is_command_valid = 0;
		strncat(pcWriteBuffer, "Parameter Not Entered",strlen("Parameter Not Entered"));
		strncat(pcWriteBuffer, "\r\n", strlen("\r\n"));
	    }


	}
    while (pcParameter != NULL);

	    /*Execute command*/

	    if (is_command_valid == 1 && param_received == 1)
		{
		uint32_t millis = HAL_GetTick() - millis1;
		itoa(millis, millis_int_to_str, 10); //base 10
		vSerialPutString(&huart2, (uint8_t*) millis_int_to_str, strlen(millis_int_to_str));
		}

    return pdFALSE;

    }

const CLI_Command_Definition_t G0_defination =
    {
    "G0", /* The command string to type. */
    "\r\nG0: Motor Move\r\n",
    &G0_callback, /* The function to run. */
    -1 /* The user can enter any number of parameter. */
    };

const CLI_Command_Definition_t g0_defination =
    {
    "g0", /* The command string to type. */
    "\r\nG0: Motor Move\r\n",
    &G0_callback, /* The function to run. */
    -1 /* The user can enter any number of parameter. */
    };
/*************************************************************************/






/*************************************************************************/

BaseType_t mconfig_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "mconfig OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t mconfig_defination =
    {
    "mconfig", /* The command string to type. */
    "\r\nmconfig: config m motor\r\n",
    &mconfig_callback, /* The function to run. */
    0
    /* No parameters are expected. */
    };
/*************************************************************************/



/*************************************************************************/

BaseType_t yconfig_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "yconfig OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t yconfig_defination =
    {
    "yconfig", /* The command string to type. */
    "\r\nyconfig: config y motor\r\n",
    &yconfig_callback, /* The function to run. */
    0
    /* No parameters are expected. */
    };
/*************************************************************************/



/*************************************************************************/

BaseType_t zconfig_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "zconfig OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t zconfig_defination =
    {
    "zconfig", /* The command string to type. */
    "\r\nzconfig: config z motor\r\n",
    &zconfig_callback, /* The function to run. */
    0
    /* No parameters are expected. */
    };
/*************************************************************************/



/*************************************************************************/

BaseType_t xconfig_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "xconfig OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t xconfig_defination =
    {
    "xconfig", /* The command string to type. */
    "\r\nxconfig: config x motor\r\n",
    &zconfig_callback, /* The function to run. */
    0
    /* No parameters are expected. */
    };
/*************************************************************************/



/*************************************************************************/

static BaseType_t prvThreeParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	//configASSERT( pcWriteBuffer );

	if( uxParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer, "The three parameters were:\r\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		uxParameterNumber = 1U;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		//configASSERT( pcParameter );

		/* Return the parameter string. */
		memset( pcWriteBuffer, 0x00, xWriteBufferLen );
		sprintf( pcWriteBuffer, "%d: ", ( int ) uxParameterNumber );
		strncat( pcWriteBuffer, pcParameter, ( size_t ) xParameterStringLength );
		strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

		/* If this is the last of the three parameters then there are no more
		strings to return after this one. */
		if( uxParameterNumber == 3U )
		{
			/* If this is the last of the three parameters then there are no more
			strings to return after this one. */
			xReturn = pdFALSE;
			uxParameterNumber = 0;
		}
		else
		{
			/* There are more parameters to return after this one. */
			xReturn = pdTRUE;
			uxParameterNumber++;
		}
	}

	return xReturn;
}

/* Structure that defines the "echo_3_parameters" command line command.  This
takes exactly three parameters that the command simply echos back one at a
time. */
static const CLI_Command_Definition_t xThreeParameterEcho =
{
	"echo-3-parameters",
	"\r\necho-3-parameters <param1> <param2> <param3>:\r\n Expects three parameters, echos each in turn\r\n",
	prvThreeParameterEchoCommand, /* The function to run. */
	3 /* Three parameters are expected, which can take any value. */
};
/*************************************************************************/



/*************************************************************************/

static BaseType_t prvParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	//configASSERT( pcWriteBuffer );

	if( uxParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer, "The parameters were:\r\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		uxParameterNumber = 1U;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		if( pcParameter != NULL )
		{
			/* Return the parameter string. */
			memset( pcWriteBuffer, 0x00, xWriteBufferLen );
			sprintf( pcWriteBuffer, "%d: ", ( int ) uxParameterNumber );
			strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
			strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

			/* There might be more parameters to return after this one. */
			xReturn = pdTRUE;
			uxParameterNumber++;
		}
		else
		{
			/* No more parameters were found.  Make sure the write buffer does
			not contain a valid string. */
			pcWriteBuffer[ 0 ] = 0x00;

			/* No more data to return. */
			xReturn = pdFALSE;

			/* Start over the next time this command is executed. */
			uxParameterNumber = 0;
		}
	}

	return xReturn;
}


/* Structure that defines the "echo_parameters" command line command.  This
takes a variable number of parameters that the command simply echos back one at
a time. */
static const CLI_Command_Definition_t xParameterEcho =
{
	"echo-parameters",
	"\r\necho-parameters <...>:\r\n Take variable number of parameters, echos each in turn\r\n",
	prvParameterEchoCommand, /* The function to run. */
	-1 /* The user can enter any number of commands. */
};
/*************************************************************************/







void CLI_Commands_Register()
    {
    FreeRTOS_CLIRegisterCommand(&G28_defination);
    FreeRTOS_CLIRegisterCommand(&G1_defination);

    FreeRTOS_CLIRegisterCommand(&G0_defination);
    FreeRTOS_CLIRegisterCommand(&g0_defination);

    FreeRTOS_CLIRegisterCommand(&xParameterEcho);
    FreeRTOS_CLIRegisterCommand(&xThreeParameterEcho);
    FreeRTOS_CLIRegisterCommand(&xconfig_defination);
    FreeRTOS_CLIRegisterCommand(&zconfig_defination);
    FreeRTOS_CLIRegisterCommand(&yconfig_defination);
    FreeRTOS_CLIRegisterCommand(&mconfig_defination);

    }



