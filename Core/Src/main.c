/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usbd_hid_if.h"
#include "usbd_midi_if.h"
#include "key.h"
#include "keymat.h"
#include "encoder.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define KEY_PRESSED  GPIO_PIN_SET
#define KEY_RELEASED GPIO_PIN_RESET

#define NUM_MATKEYS 1u

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch2_ch4;

/* USER CODE BEGIN PV */

GPIO_Pin keymat_rows_def[KEYMAT_ROWS];
GPIO_Pin keymat_cols_def[KEYMAT_COLS];
MatKey* keymat_keys_def[NUM_MATKEYS];
GPIOKey* gpio_key_def;
Encoder* encoder_def;

uint16_t ctrlState = 0x0000;
uint8_t midiState = 0xff;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

void OnKeyStateChanged(Key* sender, uint8_t oldState, uint8_t newState);
void OnEncoderTicked(Encoder* sender, int8_t direction, uint8_t edge);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void OnKeyStateChanged(Key* sender, uint8_t oldState, uint8_t newState) {
  if (newState == KEY_PRESSED) {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET);
    ctrlState = ctrlState | CTRL_PLAY_PAUSE;
    USBD_HID_SendCtrlReport_FS(ctrlState);
  } else {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET);
    ctrlState = ctrlState & ~CTRL_PLAY_PAUSE;
    USBD_HID_SendCtrlReport_FS(ctrlState);
  }
}

void OnEncoderTicked(Encoder* sender, int8_t direction, uint8_t edge) {
  if (direction > 0) {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET);
    if(midiState < 0xff) {
      midiState++;
    }
    USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, midiState);
  } else {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET);
    if(midiState > 0x00) {
      midiState--;
    }
    USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, midiState);
  }
}

GPIO_Pin keymat_rows_def[KEYMAT_ROWS] = {
  {
    .GPIOx = ROW_0_GPIO_Port,
    .GPIO_Pin = ROW_0_Pin,
  },
  {
    .GPIOx = ROW_1_GPIO_Port,
    .GPIO_Pin = ROW_1_Pin,
  },
  {
    .GPIOx = ROW_2_GPIO_Port,
    .GPIO_Pin = ROW_2_Pin,
  },
  {
    .GPIOx = ROW_3_GPIO_Port,
    .GPIO_Pin = ROW_3_Pin,
  }
};

GPIO_Pin keymat_cols_def[KEYMAT_COLS] = {
  {
    .GPIOx = COL_0_GPIO_Port,
    .GPIO_Pin = COL_0_Pin,
  },
  {
    .GPIOx = COL_1_GPIO_Port,
    .GPIO_Pin = COL_1_Pin,
  },
  {
    .GPIOx = COL_2_GPIO_Port,
    .GPIO_Pin = COL_2_Pin,
  },
  {
    .GPIOx = COL_3_GPIO_Port,
    .GPIO_Pin = COL_3_Pin,
  }
};

MatKey* keymat_keys_def[NUM_MATKEYS] = {
  &((MatKey){
    .Key = &((Key){
      .State = KEY_RELEASED,
      .OnStateChanged = OnKeyStateChanged,
    }),
    .X = 0,
    .Y = 0,
  }),
};

GPIOKey* gpio_key_def = &((GPIOKey){
  .Key = &((Key){
    .State = KEY_RELEASED,
    .OnStateChanged = OnKeyStateChanged,
  }),
  .GPIOx = ENC_2_P_GPIO_Port,
  .GPIO_Pin = ENC_2_P_Pin,
});

Encoder* encoder_def = &((Encoder){
  .GPIOx_A = ENC_1_A_GPIO_Port,
  .GPIO_Pin_A = ENC_1_A_Pin,
  .GPIOx_B = ENC_1_B_GPIO_Port,
  .GPIO_Pin_B = ENC_1_B_Pin,
  .OnTicked = OnEncoderTicked,
});

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
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  GPIOKey_Init(gpio_key_def);
  Keymat_Init(keymat_rows_def, keymat_cols_def, keymat_keys_def, NUM_MATKEYS);
  Encoder_Init(encoder_def);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // USB test
    // HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET);
    // // USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, 0x00);
    // // USBD_HID_SendCtrlReport_FS(CTRL_PLAY_PAUSE);
    // HAL_Delay(500);
    // HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET);
    // // USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, 0xff);
    // // USBD_HID_SendCtrlReport_FS(0);
    // HAL_Delay(500);

    // key matrix test
    Keymat_Scan();
    // single key test
    GPIOKey_Scan(gpio_key_def);
    // encoder test
    Encoder_Scan(encoder_def);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 90-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, STATE_LED_Pin|ROW_3_Pin|ROW_2_Pin|ROW_1_Pin
                          |ROW_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENC_1_B_Pin ENC_1_A_Pin COL_3_Pin */
  GPIO_InitStruct.Pin = ENC_1_B_Pin|ENC_1_A_Pin|COL_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ENC_2_A_Pin ENC_2_B_Pin ENC_2_P_Pin COL_2_Pin
                           COL_1_Pin COL_0_Pin */
  GPIO_InitStruct.Pin = ENC_2_A_Pin|ENC_2_B_Pin|ENC_2_P_Pin|COL_2_Pin
                          |COL_1_Pin|COL_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : STATE_LED_Pin */
  GPIO_InitStruct.Pin = STATE_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STATE_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW_3_Pin ROW_2_Pin ROW_1_Pin ROW_0_Pin */
  GPIO_InitStruct.Pin = ROW_3_Pin|ROW_2_Pin|ROW_1_Pin|ROW_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
