/*
 * removed.c
 *
 *  Created on: 17-Oct-2018
 *      Author: medprime
 */

#if 0


BaseType_t G0_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    const char *pcParameter;
    BaseType_t xParameterStringLength, xReturn;
    static UBaseType_t uxParameterNumber = 0;

    static char axis_found = 0;

    /* Remove compile time warnings about unused parameters, and check the
     write buffer is not NULL.  NOTE - for simplicity, this example assumes the
     write buffer length is adequate, so does not check for buffer overflows. */
    (void) pcCommandString;
    (void) xWriteBufferLen;

    if (uxParameterNumber == 0)
	{
	/* The first time the function is called after the command has been
	 entered just a header string is returned. */
	strcpy(pcWriteBuffer, "G0 OK\r\n");

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
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, /* The command string itself. */
	                                       uxParameterNumber, /* Return the next parameter. */
	                                       &xParameterStringLength /* Store the parameter string length. */
	                                       );

	if (pcParameter != NULL)
	    {
	    /* Return the parameter string. */
	    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
	    strncat(pcWriteBuffer, (char *) pcParameter,
		    (size_t) xParameterStringLength);
	    strncat(pcWriteBuffer, "\r\n", strlen("\r\n"));

	    for (uint8_t i = 0; i < NO_OF_MOTORS; i++)
	        {
	    if(pcWriteBuffer[0] == motor_name[i] || pcWriteBuffer[0] == motor_name_caps[i])
	        {
	    	motor_steps[i] = atof(pcWriteBuffer+1);
	    	axis_found ++ ;
	        }
	    }

	    /* There might be more parameters to return after this one. */
	    xReturn = pdTRUE;
	    uxParameterNumber++;
	    }
	else
	    {
	    /* No more parameters were found.  Make sure the write buffer does
	     not contain a valid string. */
            if(axis_found == 0)
        	{
            strcpy(pcWriteBuffer, "Wrong Parameters Entered.\r\nValid Values are x,y,z,m or X,Y,Z,M.\r\n");
        	}
            else
        	{
        	pcWriteBuffer[0] = 0x00;
        	}

            axis_found = 0; //reset

	    /* No more data to return. */
	    xReturn = pdFALSE;

	    /* Start over the next time this command is executed. */
	    uxParameterNumber = 0;
	    }
	}

    return xReturn;
    }



/*************************************************************************/

const char* wrong_parameter_str = "Wrong Parameters Entered.\r\nValid Values are x,y,z,m or X,Y,Z,M.\r\n";

BaseType_t G0_callback1(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    static char clear_buffer_flag = 0;
    static uint8_t first_wrong_axis = 0;



    /* Remove compile time warnings about unused parameters, and check the
     write buffer is not NULL.  NOTE - for simplicity, this example assumes the
     write buffer length is adequate, so does not check for buffer overflows. */
    (void) pcCommandString;
    (void) xWriteBufferLen;

    if(clear_buffer_flag == 1)
	{
	clear_buffer_flag = 0; //buffer cleared
	pcWriteBuffer[0] = '\0';
	first_wrong_axis = 0;
        return pdFALSE; // Don't return to this function again
	}

	/* Obtain the parameter string. */
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, /* The command string itself. */
	                                       1, /* Return the next parameter. */
	                                       &xParameterStringLength /* Store the parameter string length. */
	                                       );

    if (pcParameter != NULL)
	{

	uint8_t axis_found = 0;
	const char* axis_temp = NULL;

	strncat(pcWriteBuffer,"G0 OK\r\n",strlen("G0 OK\r\n"));

	while (*pcParameter != '\0')
	    {

	    axis_found = 0;
	    axis_temp = pcParameter;

	    for (uint8_t i = 0; i < NO_OF_MOTORS; i++)
		{
		if ( *pcParameter== motor_name[i] || *pcParameter == motor_name_caps[i]) // valid axis xyzm or XYZM
		    {

		    axis_found = 1; //valid axix found

		    if (str_to_int((pcParameter + 1), &motor_steps[i]) == 1)
			{
			strncat(pcWriteBuffer,&motor_name_caps[i],1);
			strncat(pcWriteBuffer,"=",1);
			while(*pcParameter  != '\0' && *pcParameter != ' ')
			    {
			    pcParameter++;
		            strncat(pcWriteBuffer,pcParameter,1);
			    }
		        strncat(pcWriteBuffer,"\r\n",strlen("\r\n"));
			}
		    else
			{
			strncat(pcWriteBuffer, "Invalid argument for ", strlen("Invalid argument for "));
			strncat(pcWriteBuffer, &motor_name_caps[i], 1);
			strncat(pcWriteBuffer, " motor. --> ", strlen(" motor. --> "));
			strncat(pcWriteBuffer, &motor_name_caps[i], 1);
			while(*pcParameter  != '\0' && *pcParameter != ' ')
			    {
			    pcParameter++;
		            strncat(pcWriteBuffer,pcParameter,1);
			    }
		        strncat(pcWriteBuffer,"\r\n",strlen("\r\n"));

			}
		    }

		}

		if (axis_found == 0) // one of the axis was invalid.
		    {
		    if(first_wrong_axis ==0)
			{
			first_wrong_axis = 1;
		    strncat(pcWriteBuffer,wrong_parameter_str, strlen(wrong_parameter_str));
			}
		    strncat(pcWriteBuffer, "--->", strlen("--->"));
		    while (*axis_temp != '\0' && *axis_temp != ' ')
			{
			strncat(pcWriteBuffer, axis_temp, 1);
			axis_temp++;
			}
		    strncat(pcWriteBuffer, "\r\n", strlen("\r\n"));

		    }

	    pcParameter++;
	    }

        clear_buffer_flag = 1; //need to clear pcWriteBuffer
        return pdTRUE; //return to this function again

	}
	else // zero parameter entered
	    {
            strcpy(pcWriteBuffer, "No Parameters Entered.\r\nValid Values are x,y,z,m or X,Y,Z,M.\r\n");
            clear_buffer_flag = 1; //need to clear pcWriteBuffer
            return pdTRUE; //return to this function again
	    }

    }




BaseType_t G0_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    const char * invalid_parameter = "Invalid parameter ";
    const char *pcParameter;
    BaseType_t xParameterStringLength, xReturn;
    static UBaseType_t uxParameterNumber = 0;
    uint16_t motor_steps[NO_OF_MOTORS] = {0};
    uint8_t  motor_direction[NO_OF_MOTORS] = {0};
    static uint8_t is_command_valid = 1;  //reset if any param is invalid
    static uint8_t param_received = 0; //set if at least one param is valid

    /* Remove compile time warnings about unused parameters, and check the
     write buffer is not NULL.  NOTE - for simplicity, this example assumes the
     write buffer length is adequate, so does not check for buffer overflows. */
    (void) pcCommandString;
    (void) xWriteBufferLen;

    if (uxParameterNumber == 0)
	{
	/* The first time the function is called after the command has been
	 entered just a header string is returned. */
	sprintf(pcWriteBuffer, "G0 Ok:\r\n");

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
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, /* The command string itself. */
	uxParameterNumber, /* Return the next parameter. */
	&xParameterStringLength /* Store the parameter string length. */
	);

	if (pcParameter != NULL)
	    {

	    memset(pcWriteBuffer, 0x00, xWriteBufferLen);

	    switch (*pcParameter)
		{

	    case 'x':
	    case 'X':
		{
		if (str_to_int((pcParameter + 1), &motor_steps[0], &motor_direction[0]) == 1)
		    {
		    // string to int success
		    param_received = 1;
		    sprintf(pcWriteBuffer, "X=");
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for x
		    is_command_valid = 0;
		    sprintf(pcWriteBuffer, invalid_parameter);
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
		    sprintf(pcWriteBuffer, "Y=");
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for y
		    is_command_valid = 0;
		    sprintf(pcWriteBuffer, invalid_parameter);
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
		    sprintf(pcWriteBuffer, "Z=");
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for z
		    is_command_valid = 0;
		    sprintf(pcWriteBuffer, invalid_parameter);
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
		    sprintf(pcWriteBuffer, "M=");
		    strncat(pcWriteBuffer, (pcParameter + 1), (size_t) (xParameterStringLength - 1));
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		else
		    {
		    //invalid argument for m
		    is_command_valid = 0;
		    sprintf(pcWriteBuffer, invalid_parameter);
		    strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		    strncat(pcWriteBuffer, "\r\n", 3);
		    }
		}
		break;

	    default:
		{
		//invalid parameter(s)
		param_received = 0;
		is_command_valid = 0;
		sprintf(pcWriteBuffer, invalid_parameter);
		strncat(pcWriteBuffer, pcParameter, (size_t) xParameterStringLength);
		strncat(pcWriteBuffer, "\r\n", strlen("\r\n"));
		}

		}

	    /* There might be more parameters to return after this one. */
	    xReturn = pdTRUE;
	    uxParameterNumber++;
	    }
	else
	    {
	    /* No more parameters were found.  Make sure the write buffer does
	     not contain a valid string. */
	    pcWriteBuffer[0] = 0x00;

	    /* No more data to return. */
	    xReturn = pdFALSE;

	    /* Start over the next time this command is executed. */
	    uxParameterNumber = 0;

	    /*Execute command*/

	    if (is_command_valid == 1 && param_received == 1)
		{
		uint32_t millis = HAL_GetTick() - millis1;
		itoa(millis, millis_int_to_str, 10); //base 10
		vSerialPutString(&huart2, (uint8_t*) millis_int_to_str, strlen(millis_int_to_str));
		}

	    is_command_valid = 1; //reset state
	    param_received = 0; //reset state

	    }

	}
    return xReturn;

    }













#endif
