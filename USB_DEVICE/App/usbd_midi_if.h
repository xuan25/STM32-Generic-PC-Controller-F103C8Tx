/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_midi_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_MIDI_if.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MIDI_IF_H__
#define __USBD_MIDI_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_MIDI USBD_MIDI
  * @brief Usb MIDI interface device module.
  * @{
  */

/** @defgroup USBD_MIDI_Exported_Defines USBD_MIDI_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Exported_Types USBD_MIDI_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Exported_Macros USBD_MIDI_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Exported_Variables USBD_MIDI_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** MIDI Interface callback. */
// extern USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_Exported_FunctionsPrototype USBD_MIDI_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

/**
  * @brief  Handle midi event from host
  * @param  cableNumber: Cable Number (0-15)
  * @param  channelNumber: Channel Number (0-15)
  * @param  controllerNumber: Controller Number (0-127)
  * @param  value: Value (0-127)
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
uint8_t USBD_MIDI_SendCCMessage_FS(uint8_t cableNumber, uint8_t channelNumber, uint8_t controllerNumber, uint8_t value);

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_MIDI_IF_H__ */

