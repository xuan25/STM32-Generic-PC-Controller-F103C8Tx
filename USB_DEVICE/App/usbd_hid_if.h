/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_hid_if.h
  * @version        : v2.0_Cube
  * @brief          : Header for usbd_hid_if.c file.
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

#define VENDOR_REPORT_ID        1u
#define VENDOR_REPORT_LENGTH    0x40u
#define VENDOR_DATA_LENGTH    VENDOR_REPORT_LENGTH - 1  // reserve one byte for report ID

#define CTRL_REPORT_ID      2u
#define CTRL_REPORT_LENGTH  0x03u

#define CTRL_PLAY                 1u << 0x0
#define CTRL_PAUSE                1u << 0x1
#define CTRL_RECORD               1u << 0x2
#define CTRL_FAST_FORWARD         1u << 0x3
#define CTRL_REWIND               1u << 0x4
#define CTRL_SCAN_NEXT_TRACK      1u << 0x5
#define CTRL_SCAN_PREVIOUS_TRACK  1u << 0x6
#define CTRL_STOP                 1u << 0x7
#define CTRL_EJECT                1u << 0x8
#define CTRL_RANDOM_PLAY          1u << 0x9
#define CTRL_TRACK_INCREMENT      1u << 0xA
#define CTRL_TRACK_DECREMENT      1u << 0xB
#define CTRL_PLAY_PAUSE           1u << 0xC
#define CTRL_MUTE                 1u << 0xD
#define CTRL_VOLUME_INCREMENT     1u << 0xE
#define CTRL_VOLUME_DECREMENT     1u << 0xF

#define KEYBOARD_REPORT_ID      3u
#define KEYBOARD_REPORT_LENGTH  0x09u

#define MOUSE_REPORT_ID     4u
#define MOUSE_REPORT_LENGTH 0x05u

#define RADIAL_REPORT_ID     5u
#define RADIAL_REPORT_LENGTH 0x09u

#define RADIAL_HAPTIC_REPORT_ID     6u
#define RADIAL_HAPTIC_REPORT_LENGTH 0x06u

#define LIGHTING_REPORT_ID     7u
#define LIGHTING_REPORT_LENGTH 0x3eu

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

