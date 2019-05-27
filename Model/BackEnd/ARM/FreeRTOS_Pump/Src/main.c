
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);


static void idle(void);
static void vMainTaskCode( void * pvParameters );
/* Private function prototypes -----------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static void (* state_fn)(void) = idle;
static char recv[36];

UART_HandleTypeDef huart3;
DrugCureList drugCureList;

/**
  * @brief  The application entry point.
  *
  * @retval None
  */

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE END SysInit */
  InitDrugCureList(&drugCureList);

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();

  /* Create the task, storing the handle. */
  BaseType_t xReturned;
  TaskHandle_t xMainHandle = NULL;
  xReturned = xTaskCreate(
                      	  vMainTaskCode,       	/* Function that implements the task. */
						  "MainTask",          	/* Text name for the task. */
						  STACK_SIZE,      		/* Stack size in words, not bytes. */
						  ( void * ) 1,    		/* Parameter passed into the task. */
						  tskIDLE_PRIORITY,		/* Priority at which the task is created. */
						  &xMainHandle );       /* Used to pass out the created task's handle. */

  /* Start FreeRTOS scheduler */
  vTaskStartScheduler();
  
  if (xReturned == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
  {
	  perror("thread create");
  }

  /* We should never get here as control is now taken by the scheduler */
  for(;;)
  {
  }

}

/* Main Task */
static void vMainTaskCode( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. */
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    //HAL_UART_Transmit(&huart3, "Main Task\n", strlen("Main Task\n"), 0xFF);

    /*Main Task LED*/
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    /*Launching Finite State Machine*/
    while(state_fn)
    {
    	(*state_fn)();
    }
}

static void idle(void)
{
	HAL_UART_Receive_IT(&huart3, (uint8_t *) recv, 0x24);
}

void computeEntry(void)
{
	char codeOp[8];
	char name[24];
	char hours[8];
	char minutes[8];
	char volume[8];

	parse(codeOp, name, hours, minutes, volume);

	unsigned int ucodeOp = atoi(codeOp);
	unsigned int uhours = atoi(hours);
	unsigned int uminutes = atoi(minutes);
	unsigned int uvolume = atoi(volume);
	
	/*Deletion operation*/
	if( ucodeOp == 0 )
	{
		if(CheckName(&drugCureList, name) > 0)
		{
			RemoveCure(&drugCureList, name);
		}
	}

	/*Creation operation Mode*/
	else if( ucodeOp ==  1)
	{
		/*Check that the number of task is less than  3*/
		if(uxTaskGetNumberOfTasks() < 4)
		{
			AddCureDrug(&drugCureList, name, uhours, uminutes, uvolume);
			LaunchCure(&drugCureList, name);
		}
	}
	/*Modification Operation Mode*/
	else if( ucodeOp  == 2 )
	{
		if(CheckName(&drugCureList, name) > 0)
				{
					ModifyCure(&drugCureList, name, uhours, uminutes, uvolume);
				}
	}

	/*Received Wrong Packet*/
	else
	{
	}

	/*Back to idle state in the FSM*/
	state_fn = idle;
}

void parse(char * codeOp, char * name, char * hours, char * minutes, char * volume)
{
	int i;
	char *Strptr = (char *) & recv;

	for( i=0 ; i<4 ; i++, Strptr++)
		*(codeOp + i) = *(Strptr);
	*(codeOp + 4 )='\0';

	for( i=0 ; i<20 ; i++, Strptr++)
		*(name + i) = *(Strptr);
	*(name + 20) ='\0';

	for( i=0 ; i<4 ; i++, Strptr++ )
			*(hours +i) = *(Strptr);
	*(hours +4) ='\0';

	for( i=0 ; i<4 ; i++, Strptr++ )
			*(minutes + i) = *(Strptr);
	*(minutes + 5) ='\0';

	for( i=0 ; i<4 ; i++, Strptr++ )
			*(volume + i) = *(Strptr);
	*(volume + 5)='\0';
	return;
}

char * concat(int id, char * name, uint32_t count)
{
	char *ptr_buffer = (char *) malloc(sizeof(char) * 48);

	if(ptr_buffer == 0x00)
	{
		perror("malloc");
	}

	char *ptr_begin_buffer = ptr_buffer;

    char buf_name[24];
    char buf_id[8];
    char buf_count[12];


    char *ptr_id = (char *) buf_id;
    char *ptr_name = (char *) buf_name;
    char *ptr_count = (char *) buf_count;

    int i;

    sprintf(buf_name, "%s", name);
    sprintf(buf_id, "%d", id);
    //sprintf(buf_count, "%zu", count);
	sprintf(buf_count, "%" PRIu32 , count);

    if(strlen(buf_id) <= 4)
    {
        for(i=0;i<(4-strlen(buf_id));i++, ptr_buffer++)
        {
            *(ptr_buffer) = '0';
        }

        for(i=0;i<strlen(buf_id);i++, ptr_buffer++, ptr_id++)
        {
            *(ptr_buffer) = *(ptr_id);
        }
    }
    *(buf_id + 4) = '\0';

    if(strlen(buf_name) <= 20)
    {
        for(i=0;i<(20-strlen(buf_name));i++, ptr_buffer++)
        {
            *(ptr_buffer) = '0';
        }

        for(i=0;i<strlen(buf_name);i++, ptr_buffer++, ptr_name++)
        {
            *(ptr_buffer) = *(ptr_name);
        }

    }
    *(buf_name + 20) = '\0';

    if(strlen(buf_count) <= 8)
    {
        for(i=0;i<(8-strlen(buf_count));i++, ptr_buffer++)
        {
            *(ptr_buffer) = '0';
        }

        for(i=0;i<strlen(buf_count);i++, ptr_buffer++, ptr_count++)
        {
            *(ptr_buffer) = *(ptr_count);
        }
    }
    *(ptr_buffer) = '\0';

 return ptr_begin_buffer;
}

/*Uart CallBack For Changing the FSM*/
void HAL_UART_RxCpltCallback( UART_HandleTypeDef * huart )
{
	  state_fn = computeEntry;
}

/* Task Cure 1 */
void vCureTaskCodeA( void * pvParameters )
{
	char *buffer = 0x00;
    
	DrugCure *drug = pvParameters;

	int id = 1;
	int i; 

	uint32_t tickBegin;
	uint32_t tickDuration;
	uint32_t cureDuration = drug->hours * 3600 + drug->minutes*60;

	tickBegin = xTaskGetTickCount();

    for( ;; )
    {
    	for(i=0;i<5;i++)
        {
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        	vTaskDelay(25);
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
        }

    	tickDuration = xTaskGetTickCount();

    	if(( (tickDuration - tickBegin) /configTICK_RATE_HZ) <= cureDuration)
		{
    		if( xSemaphoreTake(drug->xSemaphore, ( TickType_t ) 1000 ) == pdTRUE )
    		{
    			buffer = concat(id, drug->name, ((tickDuration - tickBegin)/configTICK_RATE_HZ));
    			xSemaphoreGive(drug->xSemaphore);
    		}
    		HAL_UART_Transmit(&huart3, (uint8_t *) buffer, strlen(buffer), 0xFF);
    		free(buffer);
    		buffer = 0x00;
		}

		vTaskDelay(1000);
	}
}


/* Task Cure 2 */
void vCureTaskCodeB( void * pvParameters )
{
	char *buffer = 0x00;

	DrugCure *drug = pvParameters;

	int id = 2;
	int i;	

	uint32_t tickBegin;
	uint32_t tickDuration;
	uint32_t cureDuration = drug->hours * 3600 + drug->minutes*60;


	tickBegin = xTaskGetTickCount();

    for( ;; )
    {
	   	for(i=0;i<5;i++)
        {
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        	vTaskDelay(25);
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
        }

    	tickDuration = xTaskGetTickCount();

    	if(( (tickDuration - tickBegin) /configTICK_RATE_HZ) <= cureDuration)
		{
	   		if( xSemaphoreTake(drug->xSemaphore, ( TickType_t ) 1000 ) == pdTRUE )
    		{
	    		buffer = concat(id, drug->name, ((tickDuration - tickBegin)/configTICK_RATE_HZ));
	    		xSemaphoreGive(drug->xSemaphore);
	    	}
	    	HAL_UART_Transmit(&huart3, (uint8_t *) buffer, strlen(buffer), 0xFF);
	    	free(buffer);
	    	buffer = 0x00;

		}
	  	
		vTaskDelay(1000);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB14 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

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
