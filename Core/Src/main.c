/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"

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
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
volatile uint32_t msTicks = 0;
volatile uint32_t current_millis = 0;
volatile uint32_t right_signal_previousMillis = 0;
volatile uint32_t left_signal_previousMillis = 0;
volatile uint32_t flasher_signal_previousMillis = 0;
volatile uint32_t led_previousMillis = 0;


volatile int flasher_sent = 0;
volatile int right_signal_sent = 0;
volatile int left_signal_sent = 0;
volatile int beam_sent = 0;
volatile int high_beam_sent = 0;

volatile int right_signal_state = 0;
volatile int left_signal_state = 0;
volatile int flasher_state = 0;
volatile int led_mini_state = 0;

volatile int flasher_data;
volatile int right_signal_data;
volatile int left_signal_data;
volatile int horn_data;
volatile int brake_data;
volatile int wipers_data;
volatile int high_beam_data;
volatile int beam_data;

const int  SIGNAL_INTERVAL = 500;
const int  LED_MINI_INTERVAL = 500;
const int DEBOUNCE_COUNT = 7;

volatile int debounce_counter_flasher = 0;
volatile int debounce_counter_right_signal = 0;
volatile int debounce_counter_left_signal = 0;
volatile int debounce_counter_high_beam = 0;
volatile int debounce_counter_beam= 0;



uint8_t myMessage[20];
uint8_t cmdEnd[3] = {0xFF,0xFF,0xFF};
uint8_t coming_message[1];
uint8_t buffer[20];
uint16_t buffer_index = 0;
volatile uint8_t message_ready = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3, coming_message, sizeof(coming_message));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  current_millis = msTicks;
	  horn_data = HAL_GPIO_ReadPin(horn_in_GPIO_Port, horn_in_Pin);
	  wipers_data = HAL_GPIO_ReadPin(wipers_in_GPIO_Port, wipers_in_Pin);
	  flasher_data = HAL_GPIO_ReadPin(flasher_in_GPIO_Port, flasher_in_Pin);



	  if(flasher_data == 0)
	  {
		  debounce_counter_flasher++;
		  if(debounce_counter_flasher >= DEBOUNCE_COUNT)
		  {
			  flasher_state = 1;
			  if(debounce_counter_flasher ==DEBOUNCE_COUNT)
			  {
			  HAL_GPIO_WritePin(right_signal_out_GPIO_Port, right_signal_out_Pin, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(left_signal_out_GPIO_Port, left_signal_out_Pin, GPIO_PIN_SET);
			  }
				  if(current_millis - flasher_signal_previousMillis >= SIGNAL_INTERVAL)
			  {
				  HAL_GPIO_TogglePin(right_signal_out_GPIO_Port, right_signal_out_Pin);
				  HAL_GPIO_TogglePin(left_signal_out_GPIO_Port, left_signal_out_Pin);
				  flasher_signal_previousMillis = current_millis;
			  }
			  if(!flasher_sent)
			  {

				  sprintf((char*)myMessage, "flash.pic=7\n");
				  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
				  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
				  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  flasher_sent= 1;
			  }

		  }
	  }
	  else
	  {
		  flasher_state = 0;
		  debounce_counter_flasher = 0;
		  flasher_signal_previousMillis = current_millis;
		  if(flasher_sent)
		  {

			  sprintf((char*)myMessage, "flash.pic=6\n");
			  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
			  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
			  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  HAL_GPIO_WritePin(right_signal_out_GPIO_Port, right_signal_out_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(left_signal_out_GPIO_Port, left_signal_out_Pin, GPIO_PIN_RESET);

			  flasher_sent= 0;
		  }
	  }


	  right_signal_data = HAL_GPIO_ReadPin(right_signal_in_GPIO_Port, right_signal_in_Pin);
	  if(right_signal_data == 0 && flasher_state == 0)
	  {
		  debounce_counter_right_signal++;
		  if(debounce_counter_right_signal >= DEBOUNCE_COUNT)
		  {
			  //HAL_GPIO_WritePin(right_signal_out_GPIO_Port, right_signal_out_Pin, GPIO_PIN_SET);
			  if(debounce_counter_right_signal == DEBOUNCE_COUNT)
			  {
				  HAL_GPIO_WritePin(right_signal_out_GPIO_Port, right_signal_out_Pin, GPIO_PIN_SET);
			  }
			  if(current_millis - right_signal_previousMillis >= SIGNAL_INTERVAL)
			  {
				  HAL_GPIO_TogglePin(right_signal_out_GPIO_Port, right_signal_out_Pin);
				  right_signal_previousMillis = current_millis;
			  }
			  if(!right_signal_sent)
			  {
				  sprintf((char*)myMessage, "right.pic=10\n");
				  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
				  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
				  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  right_signal_sent = 1;
			  }
		  }
	  }
	  else
	  {

	  right_signal_previousMillis = current_millis;
	  debounce_counter_right_signal = 0;
	  if(right_signal_sent)
	  	  {
		  sprintf((char*)myMessage, "right.pic=11\n");
		  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
		  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
		  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
		  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
		  HAL_GPIO_WritePin(right_signal_out_GPIO_Port, right_signal_out_Pin, GPIO_PIN_RESET);
		  right_signal_sent = 0;
	  	  }
	  }



	  left_signal_data = HAL_GPIO_ReadPin(left_signal_in_GPIO_Port, left_signal_in_Pin);
	  if(left_signal_data == 0 && flasher_state == 0)
	  {
		  debounce_counter_left_signal++;
		  if(debounce_counter_left_signal >= DEBOUNCE_COUNT)
		  {
			  //HAL_GPIO_WritePin(left_signal_out_GPIO_Port, left_signal_out_Pin, GPIO_PIN_SET);
			  if(debounce_counter_left_signal == DEBOUNCE_COUNT)
			  {
				  HAL_GPIO_WritePin(left_signal_out_GPIO_Port, left_signal_out_Pin, GPIO_PIN_SET);
			  }
			  if(current_millis - left_signal_previousMillis >= SIGNAL_INTERVAL)
			  {
				  HAL_GPIO_TogglePin(left_signal_out_GPIO_Port, left_signal_out_Pin);
				  left_signal_previousMillis = current_millis;

			  }
			  if(!left_signal_sent)
			  {

				  sprintf((char*)myMessage, "left.pic=9\n");
				  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
				  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
				  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  left_signal_sent = 1;
			  }
		  }
	  }
	  else
	  {

	  left_signal_previousMillis = current_millis;
	  debounce_counter_left_signal = 0;
	  if(left_signal_sent)
	  	  {
		  sprintf((char*)myMessage, "left.pic=8\n");
		  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
		  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
		  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
		  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
		  left_signal_sent = 0;
		  HAL_GPIO_WritePin(left_signal_out_GPIO_Port, left_signal_out_Pin, GPIO_PIN_RESET);

	  	  }
	  }


	  beam_data = HAL_GPIO_ReadPin(beam_lights_in_GPIO_Port, beam_lights_in_Pin);
	  if(beam_data == 0)
	  {
		  debounce_counter_beam++;
		  if(debounce_counter_beam >= DEBOUNCE_COUNT)
		  {
			  HAL_GPIO_WritePin(beam_lights_out_GPIO_Port, beam_lights_out_Pin, GPIO_PIN_SET);
			  if(!beam_sent)
			  {
				  sprintf((char*)myMessage, "beam.pic=13\n");
				  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
				  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
				  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
				  beam_sent = 1;
				  HAL_Delay(10);
			  }
		  }
	  }
	  else
	  {
		  debounce_counter_beam = 0;
		  HAL_GPIO_WritePin(beam_lights_out_GPIO_Port, beam_lights_out_Pin, GPIO_PIN_RESET);
		  if(beam_sent)
		  {
			  sprintf((char*)myMessage, "beam.pic=12\n");
			  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
			  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
			  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  beam_sent = 0;
			  HAL_Delay(10);
		  }
	  }

	  high_beam_data = HAL_GPIO_ReadPin(high_beam_in_GPIO_Port, high_beam_in_Pin);
	  if(high_beam_data == 0)
	  {
		  debounce_counter_high_beam++;
		  if(debounce_counter_high_beam >= DEBOUNCE_COUNT)
		  {
	  		  HAL_GPIO_WritePin(high_beam_out_GPIO_Port,high_beam_out_Pin, GPIO_PIN_SET);

		  if(!high_beam_sent)
		  {
			  sprintf((char*)myMessage, "hbeam.pic=14\n");
			  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
			  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
			  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);

			  high_beam_sent = 1;
		  }}
	  }
	  else
	  {
		debounce_counter_high_beam = 0;
	  	HAL_GPIO_WritePin(high_beam_out_GPIO_Port, high_beam_out_Pin, GPIO_PIN_RESET);
	  	if(high_beam_sent)
	  	{
			  sprintf((char*)myMessage, "hbeam.pic=15\n");
			  HAL_UART_Transmit_IT(&huart3, myMessage, strlen((char*)myMessage));
			  while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  //HAL_UART_Transmit_IT(&huart3, cmdEnd, sizeof(cmdEnd));
			  //while(HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY);
			  high_beam_sent = 0;
	  	}
	  }


	  if(current_millis - led_previousMillis >= LED_MINI_INTERVAL)
	  {

		 if(led_mini_state == 1 )
		 {
			 HAL_GPIO_WritePin(led_mini1_GPIO_Port, led_mini1_Pin, GPIO_PIN_RESET);
			 //HAL_GPIO_WritePin(led_mini2_GPIO_Port, led_mini2_Pin, GPIO_PIN_SET);
			 led_mini_state = 0;
		 }
		 else
		 {
			 HAL_GPIO_WritePin(led_mini1_GPIO_Port, led_mini1_Pin, GPIO_PIN_SET);
			 //HAL_GPIO_WritePin(led_mini2_GPIO_Port, led_mini2_Pin, GPIO_PIN_RESET);
			 led_mini_state = 1;
		 }
		 led_previousMillis = current_millis;

	  }

	  HAL_Delay(10);



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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led_mini1_GPIO_Port, led_mini1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, led_mini2_Pin|high_beam_out_Pin|left_signal_out_Pin|right_signal_out_Pin
                          |brake_lights_out_Pin|wipers_out_Pin|horn_out_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(beam_lights_out_GPIO_Port, beam_lights_out_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led_mini1_Pin */
  GPIO_InitStruct.Pin = led_mini1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(led_mini1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : led_mini2_Pin high_beam_out_Pin left_signal_out_Pin right_signal_out_Pin
                           brake_lights_out_Pin wipers_out_Pin horn_out_Pin */
  GPIO_InitStruct.Pin = led_mini2_Pin|high_beam_out_Pin|left_signal_out_Pin|right_signal_out_Pin
                          |brake_lights_out_Pin|wipers_out_Pin|horn_out_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : high_beam_in_Pin beam_lights_in_Pin horn_in_Pin left_signal_in_Pin
                           wipers_in_Pin right_signal_in_Pin flasher_in_Pin hand_brake_in_Pin
                           brake_lights_in_Pin */
  GPIO_InitStruct.Pin = high_beam_in_Pin|beam_lights_in_Pin|horn_in_Pin|left_signal_in_Pin
                          |wipers_in_Pin|right_signal_in_Pin|flasher_in_Pin|hand_brake_in_Pin
                          |brake_lights_in_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : beam_lights_out_Pin */
  GPIO_InitStruct.Pin = beam_lights_out_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(beam_lights_out_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        if (buffer_index < sizeof(buffer) - 1) // String sonlandırma karakteri için yer bırak
        {
            if (coming_message[0] == '\n')
            {
                //buffer[buffer_index] = '\0'; // String'i sonlandır

                // Mesajın işlenmeye hazır olduğunu işaretleyin
                //message_ready = 1;

                // Mesaj kontrolü ve röle kontrolü
                if (strcmp((char *)buffer, "ON") == 0)
                {
                    // Röleyi aç
                    HAL_GPIO_WritePin(horn_out_GPIO_Port,horn_out_Pin, GPIO_PIN_SET);
                }
                else if (strcmp((char *)buffer, "OFF") == 0)
                {
                    // Röleyi kapat
                    HAL_GPIO_WritePin(horn_out_GPIO_Port,horn_out_Pin, GPIO_PIN_RESET);
                }

                // Buffer'ı sıfırla
                buffer_index = 0;
            }
            else
            {
                // Karakteri buffer'a ekle
                buffer[buffer_index++] = coming_message[0];
            }
        }
        else
        {
            // Buffer taşması, buffer'ı sıfırla
            buffer_index = 0;
        }

        // UART receive interrupt'u tekrar etkinleştir
        HAL_UART_Receive_IT(&huart3, coming_message, 1);
    }
}

/* USER CODE END 4 */

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
