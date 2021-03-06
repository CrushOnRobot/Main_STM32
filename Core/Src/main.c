/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t mode = 'a';
uint8_t rpi = 'a';
uint8_t rpiBuf[2] = {'a', 'a'};
uint8_t rx_debug;
uint8_t rx_camera;
int ccr1 = 0;
int ccr2 = 799;
int ccr3 = 1999;
short isSameMode = 0;
short isSameRpi = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int __io_putchar(int ch);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch){
	while(HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 30000) != HAL_OK){}
	return ch;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t*) &rx_debug, 1);
  HAL_UART_Receive_IT(&huart3, (uint8_t*) &rx_camera, 1);

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //servo

  printf("Initial setup finished\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(!isSameMode){
			isSameMode = 1;
			if(mode == 'a') //stop
			{
				TIM3->CCR1 = ccr1;
				TIM3->CCR4 = ccr1;
				TIM2->CCR1 = ccr1;
				TIM2->CCR4 = ccr1;
				TIM2->CCR2 = ccr1;
				TIM2->CCR3 = ccr1;
			}

			else if(mode == 'b') //clockwise
			{
				TIM3->CCR1 = ccr1;
				TIM3->CCR4 = ccr2;
				TIM2->CCR1 = ccr1;
				TIM2->CCR4 = ccr2;
				TIM2->CCR2 = ccr1;
				TIM2->CCR3 = ccr2;
			}
			else if(mode == 'c') //counter-clockwise
			{
				TIM3->CCR1 = ccr2;
				TIM3->CCR4 = ccr1;
				TIM2->CCR1 = ccr2;
				TIM2->CCR4 = ccr1;
				TIM2->CCR2 = ccr2;
				TIM2->CCR3 = ccr1;
			}
			else if(mode == 'd') //backward
			{
				TIM3->CCR1 = ccr1;
				TIM3->CCR4 = ccr1;
				TIM2->CCR1 = ccr1;
				TIM2->CCR4 = ccr2;
				TIM2->CCR2 = ccr2;
				TIM2->CCR3 = ccr1;
			}
			else if(mode == 'e') //forward
			{
				TIM3->CCR1 = ccr1;
				TIM3->CCR4 = ccr1;
				TIM2->CCR1 = ccr2;
				TIM2->CCR4 = ccr1;
				TIM2->CCR2 = ccr1;
				TIM2->CCR3 = ccr2;
			}
			else if(mode == 'x') //neck up
			{
				for(int i = 0; i < 120; i++){
					  TIM4->CCR3 = i;
					  HAL_Delay(10);
				}
			}
			else if(mode == 'y') //neck down
			{
				for(int i = 120; i > 0; i--){
					  TIM4->CCR3 = i;
					  HAL_Delay(10);
				}
			}
			else
			{
				mode = 'a';
				isSameMode = 0;
			}

			printf("mode = %c\n", mode);
	  }

	  if(!isSameRpi)
	  {
		  isSameRpi = 1;
		  if(rpi == 'h')
		  {
			  printf("!!HELP!!\n");
		  }

		  if(rpi == 'r')
		  {
			  printf("Turn Right\n");
		  }
		  else if(rpi == 'l')
		  {
			  printf("Turn Left\n");
		  }
		  else if(rpi == 'f')
		  {
			  printf("Stay Front\n");
		  }

		  if(rpi == 'u')
		  {
			  if(TIM4->CCR3 > 115) // unsigned int ????????? ?????? -> ??? ????????? ???
			  {
				  TIM4->CCR3 = 120;
			  }
			  else
			  {
				  printf("Neck Up");
				  for(int i = 0; i < 5; i++)
				  {
					  TIM4->CCR3 += 1;
					  HAL_Delay(40);
				  }
			  }

		  }
		  else if(rpi == 'd')
		  {
			  if(TIM4->CCR3 < 5)
			  {
				  TIM4->CCR3 = 0;
			  }
			  else
			  {
				  printf("Neck Down");
				  for(int i = 0; i < 5; i++)
				  {
					  TIM4->CCR3 -= 1;
					  HAL_Delay(40);
				  }
			  }
		  }
		  else if(rpi == 's'){
			  printf("Neck Stop");
		  }
	  }





    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart -> Instance == USART1){
		isSameMode = 0;
		mode = rx_debug;
		printf("====mode changed====\n");
		HAL_UART_Receive_IT(&huart1, (uint8_t*) &rx_debug, 1);
	}
	else if(huart -> Instance == USART3){
		rpiBuf[0] = rpiBuf[1];
		rpiBuf[1] = rx_camera;
		if(rpiBuf[0] != rpiBuf[1])
		{
			isSameRpi = 0;
		}
		rpi = rx_camera;
		HAL_UART_Receive_IT(&huart3, (uint8_t*) &rx_camera, 1);
	}
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
