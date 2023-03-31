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
#include "dma.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usbd_hid_if.h"
#include "usbd_midi_if.h"
#include "binary_push_key.h"
#include "key_matrix.h"
#include "encoder.h"
#include "dial.h"
#include "pushable_dial.h"
#include "ws2812b.h"
#include <math.h>
#include "rgb.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define KEY_PRESSED  GPIO_PIN_SET
#define KEY_RELEASED GPIO_PIN_RESET

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t easingDuration = 300;

// inputs

uint16_t ctrlState = 0x0000;
uint8_t midiState = 0xff;
uint8_t dialBtn = 0x0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// lighting

WS2812BSeries* ws2812bSeries = &(WS2812BSeries) {
  .Series = (WS2812B*[]){
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_RELEASED,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_1_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_2_A,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = 0.3 * ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_2_A,
                .Filter = &(Filter){
                  .Function = AlphaFilter,
                  .Params = &(AlphaFilterParams) {
                    .Alpha = 0
                  }
                }
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_3_A,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = 0.3 * ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1500,
              .EasingFrom = &(Color) {
                .RGB = RGB_3_A,
                .Filter = &(Filter){
                  .Function = AlphaFilter,
                  .Params = &(AlphaFilterParams) {
                    .Alpha = 0
                  }
                }
              }
            }
          }
        }
      }
    },
    NULL
  },
  .TIM = &htim2,
  .TIMChannel = TIM_CHANNEL_4
};

__weak void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  WS2812BSeries_OnHT(ws2812bSeries);
}

__weak void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  WS2812BSeries_OnTC(ws2812bSeries);
}

// inputs

uint8_t OnGPIOKeyStateChanged(BinaryPushKey* sender, BinaryPushKeyState state) {
  if (state == PushKeyPressed) {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET);

    // Consumer Control
    // ctrlState = ctrlState | CTRL_PLAY_PAUSE;
    // USBD_HID_SendCtrlReport_FS(ctrlState);

    // Dial
    // dialBtn = 1;
    // USBD_HID_SendRadialReport_FS(dialBtn, 0, 0, 0, 0);

  } else {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET);

    // Consumer Control
    // ctrlState = ctrlState & ~CTRL_PLAY_PAUSE;
    // USBD_HID_SendCtrlReport_FS(ctrlState);

    // Dial
    // dialBtn = 0;
    // USBD_HID_SendRadialReport_FS(dialBtn, 0, 0, 0, 0);
    
  }

  return 0;
}

uint8_t OnMatrixKeyStateChanged(MatrixKey* sender, BinaryPushKeyState state) {
  if (state == PushKeyPressed) {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET);

    // Consumer Control
    // ctrlState = ctrlState | CTRL_PLAY_PAUSE;
    // USBD_HID_SendCtrlReport_FS(ctrlState);

    // Dial
    // dialBtn = 1;
    // USBD_HID_SendRadialReport_FS(dialBtn, 0, 0, 0, 0);

  } else {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET);

    // Consumer Control
    // ctrlState = ctrlState & ~CTRL_PLAY_PAUSE;
    // USBD_HID_SendCtrlReport_FS(ctrlState);

    // Dial
    // dialBtn = 0;
    // USBD_HID_SendRadialReport_FS(dialBtn, 0, 0, 0, 0);
    
  }

  return 1;
}

uint8_t OnKeyMatrixStateChanged(KeyMatrix* sender, MatrixKey* matrixKey, BinaryPushKeyState state) {
  printf("Unhandled MatrixKey event");
  return 0;
}

uint8_t OnEncoderTicked(Encoder* sender, int8_t direction, Encoder_Edge edge) {
  if (direction > 0) {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET);

    // MIDI-CC
    // if(midiState < 0xff) {
    //   midiState++;
    // }
    // USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, midiState);

    // Dial
    // USBD_HID_SendRadialReport_FS(dialBtn, 10, 0, 0, 0);

  } else {
    HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET);

    // MIDI-CC
    // if(midiState > 0x00) {
    //   midiState--;
    // }
    // USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, midiState);

    // Dial
    // USBD_HID_SendRadialReport_FS(dialBtn, -10, 0, 0, 0);
    
  }

  return 0;
}

uint8_t OnDialTicked(Dial* sender, int8_t direction) {
  // if (direction > 0) {
  //   // Consumer Control
  //   ctrlState = ctrlState | CTRL_NEXT;
  //   while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  //   ctrlState = ctrlState & ~CTRL_NEXT;
  //   while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  // } else {
  //   // Consumer Control
  //   ctrlState = ctrlState | CTRL_PREVIOUS;
  //   while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  //   ctrlState = ctrlState & ~CTRL_PREVIOUS;
  //   while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  // }
  
  return 0;
}

uint8_t OnReleasedDialTicked(PushableDial* sender, int8_t direction) {
  if (direction > 0) {
    // Consumer Control
    ctrlState = ctrlState | CTRL_NEXT;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    ctrlState = ctrlState & ~CTRL_NEXT;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  } else {
    // Consumer Control
    ctrlState = ctrlState | CTRL_PREVIOUS;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    ctrlState = ctrlState & ~CTRL_PREVIOUS;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  }

  Color* easingTo = ws2812bSeries->Series[0]->Color;
  AlphaFilterParams* alphaParams = (AlphaFilterParams*)ws2812bSeries->Series[0]->Color->Filter->Params;
  EasingFilterParams* easingParams = (EasingFilterParams*)ws2812bSeries->Series[0]->Color->Filter->Next->Params;
  Color* easingFrom = easingParams->EasingFrom;

  if(direction > 0) {
    easingFrom->RGB = RGB_TICKED_CW;
  } else {
    easingFrom->RGB = RGB_TICKED_CCW;
  }

  easingTo->RGB = RGB_RELEASED;

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = easingDuration;

  return 0;
}

uint8_t OnPressedDialTicked(PushableDial* sender, int8_t direction) {
  if (direction > 0) {
    // Consumer Control
    ctrlState = ctrlState | CTRL_VOLUME_INCREMENT;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    ctrlState = ctrlState & ~CTRL_VOLUME_INCREMENT;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  } else {
    // Consumer Control
    ctrlState = ctrlState | CTRL_VOLUME_DECREMENT;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    ctrlState = ctrlState & ~CTRL_VOLUME_DECREMENT;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  }

  Color* easingTo = ws2812bSeries->Series[0]->Color;
  AlphaFilterParams* alphaParams = (AlphaFilterParams*)ws2812bSeries->Series[0]->Color->Filter->Params;
  EasingFilterParams* easingParams = (EasingFilterParams*)ws2812bSeries->Series[0]->Color->Filter->Next->Params;
  Color* easingFrom = easingParams->EasingFrom;

  if(direction > 0) {
    easingFrom->RGB = RGB_TICKED_CW;
  } else {
    easingFrom->RGB = RGB_TICKED_CCW;
  }

  easingTo->RGB = RGB_PRESSED;

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = easingDuration;

  return 0;
}

uint8_t OnPWKeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked) {
  if (state == PushKeyReleased && !isDialTicked) {
    ctrlState = ctrlState | CTRL_PLAY_PAUSE;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    ctrlState = ctrlState & ~CTRL_PLAY_PAUSE;
    while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
  }
  
  Color* easingTo = ws2812bSeries->Series[0]->Color;
  AlphaFilterParams* alphaParams = (AlphaFilterParams*)ws2812bSeries->Series[0]->Color->Filter->Params;
  EasingFilterParams* easingParams = (EasingFilterParams*)ws2812bSeries->Series[0]->Color->Filter->Next->Params;
  Color* easingFrom = easingParams->EasingFrom;

  if(state == PushKeyPressed) {
    // evaluate color under current config first
    *RGB_1_TEMP = Color_EvaluateRGB(easingTo);
    // then update config
    easingFrom->RGB = RGB_1_TEMP;
    easingTo->RGB = RGB_PRESSED;
  } else {
    if (!isDialTicked) {
      easingFrom->RGB = RGB_CLICKED;
      easingTo->RGB = RGB_RELEASED;
    } else {
      // evaluate color under current config first
      *RGB_1_TEMP = Color_EvaluateRGB(easingTo);
      // then update config
      easingFrom->RGB = RGB_1_TEMP;
      easingTo->RGB = RGB_RELEASED;
    }
  }

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = easingDuration;

  return 0;
}

KeyMatrix* keyMatrix_def = &((KeyMatrix){
  .MatrixKeys = ((MatrixKey*[]){
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 0,
      .Y = 0,
      .OnStateChanged = OnMatrixKeyStateChanged,
    }),
    NULL
  }),
  .Rows = ((GPIO_Pin*[]){
    &((GPIO_Pin){
      .GPIOx = ROW_0_GPIO_Port,
      .GPIO_Pin = ROW_0_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = ROW_1_GPIO_Port,
      .GPIO_Pin = ROW_1_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = ROW_2_GPIO_Port,
      .GPIO_Pin = ROW_2_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = ROW_3_GPIO_Port,
      .GPIO_Pin = ROW_3_Pin,
    }),
    NULL
  }),
  .Cols = ((GPIO_Pin*[]){
    &((GPIO_Pin){
      .GPIOx = COL_0_GPIO_Port,
      .GPIO_Pin = COL_0_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = COL_1_GPIO_Port,
      .GPIO_Pin = COL_1_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = COL_2_GPIO_Port,
      .GPIO_Pin = COL_2_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = COL_3_GPIO_Port,
      .GPIO_Pin = COL_3_Pin,
    }),
    NULL
  }),
  .ReleasedLevel = GPIO_PIN_RESET,
  .OnStateChanged = OnKeyMatrixStateChanged,
});

BinaryPushKey* binaryPushKey_def = &((BinaryPushKey){
  .Key = &((Key){
    
  }),
  .Pin = &((GPIO_Pin){
    .GPIOx = ENC_2_P_GPIO_Port,
    .GPIO_Pin = ENC_2_P_Pin,
  }),
  .OnStateChanged = OnGPIOKeyStateChanged,
  .ReleasedLevel = GPIO_PIN_RESET
});

Encoder* encoder_def = &((Encoder){
  .PinA = &((GPIO_Pin){
    .GPIOx = ENC_1_A_GPIO_Port,
    .GPIO_Pin = ENC_1_A_Pin,
  }),
  .PinB = &((GPIO_Pin){
    .GPIOx = ENC_1_B_GPIO_Port,
    .GPIO_Pin = ENC_1_B_Pin,
  }),
  .OffLevel = GPIO_PIN_RESET,
  .OnTicked = OnEncoderTicked,
});

Dial* dial_def = &((Dial){
  .Encoder = &((Encoder){
    .PinA = &((GPIO_Pin){
      .GPIOx = ENC_1_A_GPIO_Port,
      .GPIO_Pin = ENC_1_A_Pin,
    }),
    .PinB = &((GPIO_Pin){
      .GPIOx = ENC_1_B_GPIO_Port,
      .GPIO_Pin = ENC_1_B_Pin,
    }),
    .OffLevel = GPIO_PIN_RESET,
  }),
  .TickInterval = 10,
  .ResetDelayMs = 500,
  .OnTicked = OnDialTicked,
});

PushableDial* pushableDial_def = &((PushableDial){
  .ReleasedDial = &((Dial){
    .Encoder = &((Encoder){
      .PinA = &((GPIO_Pin){
        .GPIOx = ENC_1_A_GPIO_Port,
        .GPIO_Pin = ENC_1_A_Pin,
      }),
      .PinB = &((GPIO_Pin){
        .GPIOx = ENC_1_B_GPIO_Port,
        .GPIO_Pin = ENC_1_B_Pin,
      }),
      .OffLevel = GPIO_PIN_RESET,
    }),
    .TickInterval = 5,
    .ResetDelayMs = 500,
  }),
  .PressedDial = &((Dial){
    .Encoder = &((Encoder){
      .PinA = &((GPIO_Pin){
        .GPIOx = ENC_1_A_GPIO_Port,
        .GPIO_Pin = ENC_1_A_Pin,
      }),
      .PinB = &((GPIO_Pin){
        .GPIOx = ENC_1_B_GPIO_Port,
        .GPIO_Pin = ENC_1_B_Pin,
      }),
      .OffLevel = GPIO_PIN_RESET,
    }),
    .TickInterval = 2,
    .ResetDelayMs = 500,
  }),
  .PushKey = &((BinaryPushKey){
    .Key = &((Key){
      
    }),
    .Pin = &((GPIO_Pin){
      .GPIOx = COL_0_GPIO_Port,
      .GPIO_Pin = COL_0_Pin,
    }),
    .ReleasedLevel = GPIO_PIN_RESET
  }),
  .OnReleasedDialTicked = OnReleasedDialTicked,
  .OnPressedDialTicked = OnPressedDialTicked,
  .OnPushKeyStateChanged = OnPWKeyStateChanged,
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  MX_USB_DEVICE_Init();

  BinaryPushKey_Init(binaryPushKey_def);
  KeyMatrix_Init(keyMatrix_def);
  Encoder_Init(encoder_def);
  Dial_Init(dial_def);
  PushableDial_Init(pushableDial_def);
  WS2812BSeries_Init(ws2812bSeries);

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
    KeyMatrix_Scan(keyMatrix_def);
    // single key test
    BinaryPushKey_Scan(binaryPushKey_def);
    // encoder test
    Encoder_Scan(encoder_def);
    // dial test
    Dial_Scan(dial_def);
    // pushable dial test
    HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_SET);
    PushableDial_Scan(pushableDial_def);
    HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_RESET);

    // lighting test
    uint32_t tickMs = HAL_GetTick();
    // rotating color
    *RGB_2_A = HSVToRGB(fmod((0.6 + (tickMs / 1000.0)) * 360, 360), 1, 1);
    *RGB_3_A = HSVToRGB(fmod((0.9 + (tickMs / 1000.0)) * 360, 360), 1, 1);
    // easing in and out
    for (int i = 1; i < 3; i++) {
      EasingFilterParams* easingParams = (EasingFilterParams*)ws2812bSeries->Series[i]->Color->Filter->Next->Params;
      if (easingParams->IsCompleted) {
        AlphaFilterParams* easeEndAlpha = (AlphaFilterParams*)ws2812bSeries->Series[i]->Color->Filter->Params;
        AlphaFilterParams* easeStartAlpha = (AlphaFilterParams*)easingParams->EasingFrom->Filter->Params;

        // swap alpha
        ws2812bSeries->Series[i]->Color->Filter->Params = easeStartAlpha;
        easingParams->EasingFrom->Filter->Params = easeEndAlpha;

        // start again
        easingParams->BeginTime = tickMs;
        easingParams->IsCompleted = 0;
      }
    }
    // push lighting update
    WS2812BSeries_PushUpdate(ws2812bSeries);
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
