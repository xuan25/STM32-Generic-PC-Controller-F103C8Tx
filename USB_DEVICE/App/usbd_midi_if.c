/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_midi_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device MIDI interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Xuan25.
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
#include "usbd_midi_if.h"
#include "usbd_midi.h"
#include "midicc_conf.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_MIDI
  * @{
  */

/** @defgroup USBD_MIDI_Private_TypesDefinitions USBD_MIDI_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Defines USBD_MIDI_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Macros USBD_MIDI_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Variables USBD_MIDI_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Exported_Variables USBD_MIDI_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_FunctionPrototypes USBD_MIDI_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

// static int8_t MIDI_Init_FS(void);
// static int8_t MIDI_DeInit_FS(void);
// static int8_t MIDI_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

// USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS =
// {
//   MIDI_ReportDesc_FS,
//   MIDI_Init_FS,
//   MIDI_DeInit_FS,
//   MIDI_OutEvent_FS
// };

/** @defgroup USBD_MIDI_Private_Functions USBD_MIDI_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_MIDI_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_MIDI_SendReport(&hUsbDeviceFS, report, len);
}
*/

/**
  * @brief  Handle midi event from host
  * @param  usbRxBuffer: The report received
  * @param  usbRxBufferLength: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
__weak void USBD_MIDI_DataOutHandler(uint8_t * usbRxBuffer, uint8_t usbRxBufferLength)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(usbRxBuffer);
  UNUSED(usbRxBufferLength);
}

uint8_t USBD_MIDI_SendCCMessage_FS(uint8_t cableNumber, uint8_t channelNumber, uint8_t controllerNumber, uint8_t value) {
  uint8_t codeIndexNumber = 0xB;
  uint8_t message_number = 0xB;
  
  uint8_t ccBuffer[4] = {
    cableNumber << 4 | codeIndexNumber,
    message_number << 4 | channelNumber,
    controllerNumber,
    value
  };
  
  return USBD_MIDI_SendReport(&hUsbDeviceFS, ccBuffer, 4);
}

/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

