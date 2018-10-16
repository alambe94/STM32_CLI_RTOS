
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "FreeRTOS_CLI.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */



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
    0
    /* No parameters are expected. */
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
    0
    /* No parameters are expected. */
    };
/*************************************************************************/


/*************************************************************************/

BaseType_t G0_callback(char *pcWriteBuffer, size_t xWriteBufferLen,
	const char *pcCommandString)
    {
    strcpy( pcWriteBuffer, "G0 OK\r\n" );
    return pdFALSE;
    }

const CLI_Command_Definition_t G0_defination =
    {
    "G0", /* The command string to type. */
    "\r\nG0: Motor Move\r\n",
    &G0_callback, /* The function to run. */
    0
    /* No parameters are expected. */
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


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Ring_Buffer_Init(&huart2);
  FreeRTOS_CLIRegisterCommand(&G28_defination);
  FreeRTOS_CLIRegisterCommand(&G1_defination);
  FreeRTOS_CLIRegisterCommand(&G0_defination);
  FreeRTOS_CLIRegisterCommand(&xconfig_defination);
  FreeRTOS_CLIRegisterCommand(&yconfig_defination);
  FreeRTOS_CLIRegisterCommand(&zconfig_defination);
  FreeRTOS_CLIRegisterCommand(&mconfig_defination);
  FreeRTOS_CLIRegisterCommand( &xThreeParameterEcho );
  FreeRTOS_CLIRegisterCommand( &xParameterEcho );


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
      UARTCommandConsoleLoop();
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
