/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENC_1_B_Pin GPIO_PIN_4
#define ENC_1_B_GPIO_Port GPIOA
#define ENC_1_A_Pin GPIO_PIN_5
#define ENC_1_A_GPIO_Port GPIOA
#define LED_DATA_Pin GPIO_PIN_11
#define LED_DATA_GPIO_Port GPIOB
#define ENC_2_A_Pin GPIO_PIN_12
#define ENC_2_A_GPIO_Port GPIOB
#define ENC_2_B_Pin GPIO_PIN_13
#define ENC_2_B_GPIO_Port GPIOB
#define ENC_2_P_Pin GPIO_PIN_14
#define ENC_2_P_GPIO_Port GPIOB
#define STATE_LED_Pin GPIO_PIN_15
#define STATE_LED_GPIO_Port GPIOB
#define COL_3_Pin GPIO_PIN_15
#define COL_3_GPIO_Port GPIOA
#define COL_2_Pin GPIO_PIN_3
#define COL_2_GPIO_Port GPIOB
#define ROW_3_Pin GPIO_PIN_4
#define ROW_3_GPIO_Port GPIOB
#define ROW_2_Pin GPIO_PIN_5
#define ROW_2_GPIO_Port GPIOB
#define ROW_1_Pin GPIO_PIN_6
#define ROW_1_GPIO_Port GPIOB
#define ROW_0_Pin GPIO_PIN_7
#define ROW_0_GPIO_Port GPIOB
#define COL_1_Pin GPIO_PIN_8
#define COL_1_GPIO_Port GPIOB
#define COL_0_Pin GPIO_PIN_9
#define COL_0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
