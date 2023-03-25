/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_hid_if.h
  * @version        : v2.0_Cube
  * @brief          : Header for usbd_hid_if.c file.
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
#ifndef __USBD_HID_IF_H__
#define __USBD_HID_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_HID USBD_HID
  * @brief Usb human interface device module.
  * @{
  */

/** @defgroup USBD_HID_Exported_Defines USBD_HID_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */

#define VENDER_REPORT_ID        1u
#define VENDER_REPORT_LENGTH    0x40u - 1   // reserve one byte for report ID

#define CTRL_REPORT_ID      2u
#define CTRL_REPORT_LENGTH  3u

#define CTRL_VOLUME_INCREMENT   0x01u
#define CTRL_VOLUME_DECREMENT   0x02u
#define CTRL_MUTE               0x04u
#define CTRL_PLAY_PAUSE         0x08u
#define CTRL_NEXT               0x10u
#define CTRL_PREVIOUS           0x20u

#define KEYBOARD_REPORT_ID      3u
#define KEYBOARD_REPORT_LENGTH  9u

#define MOUSE_REPORT_ID     4u
#define MOUSE_REPORT_LENGTH 5u

#define RADIAL_REPORT_ID     5u
#define RADIAL_REPORT_LENGTH 0x09

#define RADIAL_HAPTIC_REPORT_ID     6u
#define RADIAL_HAPTIC_REPORT_LENGTH 0x14

#define LIGHTING_REPORT_ID     7u
#define LIGHTING_REPORT_LENGTH 0x6

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_Types USBD_HID_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_Macros USBD_HID_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_Variables USBD_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** HID Interface callback. */
extern USBD_HID_ItfTypeDef USBD_HID_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_FunctionsPrototype USBD_HID_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

uint8_t USBD_HID_SendCtrlReport_FS(uint16_t ctrl);
uint8_t USBD_HID_SendKeyboardReport_FS(uint8_t modifier, uint8_t oem, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6);
uint8_t USBD_HID_SendMouseReport_FS(uint8_t buttons, uint8_t x, uint8_t y, uint8_t wheel);
uint8_t USBD_HID_SendRadialReport_FS(uint8_t button, int16_t dial, uint16_t x, uint16_t y, uint16_t width);

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

#endif /* __USBD_HID_IF_H__ */

