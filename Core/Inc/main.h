/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_mini1_Pin GPIO_PIN_2
#define led_mini1_GPIO_Port GPIOE
#define led_mini2_Pin GPIO_PIN_13
#define led_mini2_GPIO_Port GPIOB
#define high_beam_in_Pin GPIO_PIN_14
#define high_beam_in_GPIO_Port GPIOD
#define beam_lights_in_Pin GPIO_PIN_15
#define beam_lights_in_GPIO_Port GPIOD
#define horn_in_Pin GPIO_PIN_1
#define horn_in_GPIO_Port GPIOD
#define left_signal_in_Pin GPIO_PIN_2
#define left_signal_in_GPIO_Port GPIOD
#define wipers_in_Pin GPIO_PIN_3
#define wipers_in_GPIO_Port GPIOD
#define right_signal_in_Pin GPIO_PIN_4
#define right_signal_in_GPIO_Port GPIOD
#define flasher_in_Pin GPIO_PIN_5
#define flasher_in_GPIO_Port GPIOD
#define hand_brake_in_Pin GPIO_PIN_6
#define hand_brake_in_GPIO_Port GPIOD
#define brake_lights_in_Pin GPIO_PIN_7
#define brake_lights_in_GPIO_Port GPIOD
#define beam_lights_out_Pin GPIO_PIN_15
#define beam_lights_out_GPIO_Port GPIOG
#define high_beam_out_Pin GPIO_PIN_4
#define high_beam_out_GPIO_Port GPIOB
#define left_signal_out_Pin GPIO_PIN_5
#define left_signal_out_GPIO_Port GPIOB
#define right_signal_out_Pin GPIO_PIN_6
#define right_signal_out_GPIO_Port GPIOB
#define brake_lights_out_Pin GPIO_PIN_7
#define brake_lights_out_GPIO_Port GPIOB
#define wipers_out_Pin GPIO_PIN_8
#define wipers_out_GPIO_Port GPIOB
#define horn_out_Pin GPIO_PIN_9
#define horn_out_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
