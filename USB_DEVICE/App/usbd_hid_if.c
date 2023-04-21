/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device HID interface file.
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
#include "usbd_hid_if.h"

/* USER CODE BEGIN INCLUDE */

#include "lighting_conf.h"
#include "cmd_conf.h"

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

/** @addtogroup USBD_HID
  * @{
  */

/** @defgroup USBD_HID_Private_TypesDefinitions USBD_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_HID_Private_Defines USBD_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_HID_Private_Macros USBD_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_HID_Private_Variables USBD_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t HID_ReportDesc_FS[USBD_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */

  // ==== Vender Report ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                  Report ID (Vender)                                   |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  |                                         Data                                          |
    |  ...   |                                                                                       |
    |  0x40  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+

  */
  0x06, 0x00, 0xff,              // Usage Page (Vender)
  0x09, 0x01,                    // Usage (Vender)
  0xa1, 0x01,                    // Collection (Application)
  0x85, VENDER_REPORT_ID,        //   REPORT_ID (1)

  0x09, 0x01,                    //   Usage (Vender)
  0x15, 0x00,                    //   Logical Minimum (0)
  0x26, 0xff, 0x00,              //   Logical Maximum (255) (1 byte)
  0x75, 0x08,                    //   Report Size (8) (1 byte = 8 bit)
  0x95, VENDER_DATA_LENGTH,      //   Report Count (63)
  0x81, 0x00,                    //   Input (Data, Ary, Abs)
  0x09, 0x01,                    //   Usage (Vender)
  0x95, VENDER_DATA_LENGTH,      //   Report Count (63)
  0x91, 0x00,                    //   Output (Data, Ary, Abs)

  0xc0,                          // End Collection
  
  // ==== Consumer Control ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                             Report ID (Consumer Control)                              |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  | Vol Inc  | Vol Dec  |   Mute   |Play/Pause|   Next   | Previous | Reserved | Reserved |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x02  | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x0c,                    // Usage Page (Consumer Devices)
  0x09, 0x01,                    // Usage (Consumer Control)
  0xa1, 0x01,                    // Collection (Application)
  0x85, CTRL_REPORT_ID,          //   REPORT_ID (2)
  
  0x09, 0xe9,                    //   Usage (Volume Increment)
  0x09, 0xea,                    //   Usage (Volume Decrement)
  0x09, 0xe2,                    //   Usage (Mute)
  0x09, 0xcd,                    //   Usage (Play/Pause)
  0x09, 0xb5,                    //   Usage (Scan Next Track)
  0x09, 0xb6,                    //   Usage (Scan Previous Track)
  0x15, 0x00,                    //   Logical Minimum (0)
  0x25, 0x01,                    //   Logical Maximum (1) (OOC: single throw momentary switch)
  0x75, 0x01,                    //   Report Size (1)
  0x95, 0x10,                    //   Report Count (16) (16 reports in 2 bytes; each bit controlling one usage)
  0x81, 0x06,                    //   Input (Data, Variable, Relative)

  0xc0,                          // End Collection

  // ==== Keyboard ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  bit   |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                 Report ID (Keyboard)                                  |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  |                                       Modifiers                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x02  |                                          OEM                                          |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x03  |                                         Key 1                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x04  |                                         Key 2                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x05  |                                         Key 3                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x06  |                                         Key 4                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x07  |                                         Key 5                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x08  |                                         Key 6                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, KEYBOARD_REPORT_ID,      //   REPORT_ID (3)

  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1) (DV)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8) (8 report in 1 bytes; each bit controlling one modifier)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)

  0x95, 0x01,                    //   REPORT_COUNT (1)
  0x75, 0x08,                    //   REPORT_SIZE (8) (1 report in 1 bytes; OEM reserved)
  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

  0x95, 0x06,                    //   REPORT_COUNT (6)
  0x75, 0x08,                    //   REPORT_SIZE (8) (6 report in 6 bytes; each byte controlling one key)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0xDF,                    //   LOGICAL_MAXIMUM (223)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0xDF,                    //   USAGE_MAXIMUM (Reserved)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)

  0xc0,                          // END_COLLECTION

  // ==== Mouse ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                   Report ID (Mouse)                                   |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  | Button 1 | Button 2 | Button 3 | Reserved | Reserved | Reserved | Reserved | Reserved |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x02  |                                           X                                           |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x03  |                                           Y                                           |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x04  |                                         Wheel                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 54
  0x09, 0x02,                    // USAGE (Mouse)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x09, 0x01,                    //   USAGE (Pointer)
  0xa1, 0x00,                    //   COLLECTION (Physical)
  0x85, MOUSE_REPORT_ID,         //     REPORT_ID (4)

  0x05, 0x09,                    //     USAGE_PAGE (Button)
  0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
  0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1) (OOC: single throw momentary switch)
  0x95, 0x03,                    //     REPORT_COUNT (3)
  0x75, 0x01,                    //     REPORT_SIZE (1) (3 report in 3 bytes; each byte controlling one button)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)

  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x75, 0x05,                    //     REPORT_SIZE (5) (1 report in 5 bytes; filler for byte alignment)
  0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)

  0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
  0x09, 0x30,                    //     USAGE (X)
  0x09, 0x31,                    //     USAGE (Y)
  0x09, 0x38,                    //     USAGE (Wheel)
  0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
  0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127) (DV)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x95, 0x03,                    //     REPORT_COUNT (3) (3 report in 24 bytes; bytes are controlling X, Y and Wheel)
  0x81, 0x06,                    //     INPUT (Data,Var,Rel)

  0xc0,                          //   END_COLLECTION
  0xc0,                          // END_COLLECTION


  // ==== Windows radial controller ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                            Report ID (Radial Controller)                              |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  |  Button  |                              Dial                                          |
    +--------+----------+                                                                            +
    |  0x02  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x03  |                                           X                                           |
    +--------+                                                                                       +
    |  0x04  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x05  |                                           Y                                           |
    +--------+                                                                                       +
    |  0x06  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x07  |                                         Width                                         |
    +--------+                                                                                       +
    |  0x08  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
    /*                                     == Report Structure (Feature) ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                             Report ID (Haptic Feedback)                               |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  |                                     Auto Trigger                                      |
    +--------+                                                                                       +
    |  0x02  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x03  |                           Auto Trigger Associated Control                             |
    +--------+                                                                                       +
    |  0x04  |                                                                                       |
    +--------+                                                                                       +
    |  0x05  |                                                                                       |
    +--------+                                                                                       +
    |  0x06  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x07  |                                       Intensity                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x08  |                                     Repeat Count                                      |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x09  |                                   Retrigger Period                                    |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x0a  |                                 Waveform Cutoff Time                                  |
    +--------+                                                                                       +
    |  0x0b  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x0c  |                                Waveform List Ordinal 3                                |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x0d  |                                Waveform List Ordinal 4                                |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x0e  |                                Duration List Ordinal 3                                |
    +--------+                                                                                       +
    |  0x0f  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x10  |                                Duration List Ordinal 4                                |
    +--------+                                                                                       +
    |  0x11  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+

  */
   /*                                     == Report Structure (Output) ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                             Report ID (Haptic Feedback)                               |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  |                                    Manual Trigger                                     |
    +--------+                                                                                       +
    |  0x02  |                                                                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x03  |                                       Intensity                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x04  |                                     Repeat Count                                      |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x05  |                                   Retrigger Period                                    |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+

  */
  0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)          
  0x09, 0x0e,                     // USAGE (System Multi-Axis Controller)                      
  0xa1, 0x01,                     // COLLECTION (Application)

  0x85, RADIAL_REPORT_ID,         //   REPORT_ID (Radial Controller)                
  0x05, 0x0d,                     //   USAGE_PAGE (Digitizers)
  0x09, 0x21,                     //   USAGE (Puck)                 
  0xa1, 0x00,                     //   COLLECTION (Physical)
  0x05, 0x09,                     //     USAGE_PAGE (Buttons)           
  0x09, 0x01,                     //     USAGE (Button 1)
  0x95, 0x01,                     //     REPORT_COUNT (1)
  0x75, 0x01,                     //     REPORT_SIZE (1)   
  0x15, 0x00,                     //     LOGICAL_MINIMUM (0)      
  0x25, 0x01,                     //     LOGICAL_MAXIMUM (1)         
  0x81, 0x02,                     //     INPUT (Data,Var,Abs)
  0x05, 0x01,                     //     USAGE_PAGE (Generic Desktop)          
  0x09, 0x37,                     //     USAGE (Dial)
  0x95, 0x01,                     //     REPORT_COUNT (1)
  0x75, 0x0f,                     //     REPORT_SIZE (15)  
  0x55, 0x0f,                     //     UNIT_EXPONENT (-1)           
  0x65, 0x14,                     //     UNIT (Degrees, English Rotation)        
  0x36, 0xf0, 0xf1,               //     PHYSICAL_MINIMUM (-3600)         
  0x46, 0x10, 0x0e,               //     PHYSICAL_MAXIMUM (3600)      
  0x16, 0xf0, 0xf1,               //     LOGICAL_MINIMUM (-3600)      
  0x26, 0x10, 0x0e,               //     LOGICAL_MAXIMUM (3600)        
  0x81, 0x06,                     //     INPUT (Data,Var,Rel)  
  0x09, 0x30,                     //     USAGE (X)
  0x75, 0x10,                     //     REPORT_SIZE (16)                    
  0x55, 0x0d,                     //     UNIT_EXPONENT (-3)           
  0x65, 0x13,                     //     UNIT (Inch,EngLinear)        
  0x35, 0x00,                     //     PHYSICAL_MINIMUM (0)         
  0x46, 0xc0, 0x5d,               //     PHYSICAL_MAXIMUM (24000)      
  0x15, 0x00,                     //     LOGICAL_MINIMUM (0)      
  0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)      
  0x81, 0x02,                     //     INPUT (Data,Var,Abs)         
  0x09, 0x31,                     //     USAGE (Y)                    
  0x46, 0xb0, 0x36,               //     PHYSICAL_MAXIMUM (14000)      
  0x81, 0x02,                     //     INPUT (Data,Var,Abs)        
  0x05, 0x0d,                     //     USAGE_PAGE (Digitizers)
  0x09, 0x48,                     //     USAGE (Width)
  0x36, 0xb8, 0x0b,               //     PHYSICAL_MINIMUM (3000)
  0x46, 0xb8, 0x0b,               //     PHYSICAL_MAXIMUM (3000)
  0x16, 0xb8, 0x0b,               //     LOGICAL_MINIMUM (3000)    
  0x26, 0xb8, 0x0b,               //     LOGICAL_MAXIMUM (3000)      
  0x81, 0x03,                     //     INPUT (Cnst,Var,Abs)                               
  0xc0,                           //   END_COLLECTION

  0x85, RADIAL_HAPTIC_REPORT_ID,  //   REPORT_ID (Haptic Feedback)                
  0x05, 0x0e,                     //   USAGE_PAGE (Haptics)          
  0x09, 0x01,                     //   USAGE (Simple Haptic Controller)                      
  0xa1, 0x02,                     //   COLLECTION (Logical)
  0x09, 0x20,                     //     USAGE (Auto Trigger)
  0x16, 0x00, 0x10,               //     LOGICAL_MINIMUM (0x1000)    
  0x26, 0x04, 0x10,               //     LOGICAL_MAXIMUM (0x1004)
  0x95, 0x01,                     //     REPORT_COUNT (1)
  0x75, 0x10,                     //     REPORT_SIZE (16)
  0xb1, 0x02,                     //     FEATURE (Data,Var,Abs)
  0x09, 0x21,                     //     USAGE (Manual Trigger)
  0x91, 0x02,                     //     OUTPUT (Data,Var,Abs)
  0x09, 0x22,                     //     USAGE (Auto Trigger Associated Control)
  0x17, 0x37, 0x00, 0x01, 0x00,   //     LOGICAL_MINIMUM (0x00010037)    
  0x27, 0x37, 0x00, 0x01, 0x00,   //     LOGICAL_MAXIMUM (0x00010037)
  0x95, 0x01,                     //     REPORT_COUNT (1)
  0x75, 0x20,                     //     REPORT_SIZE (32)
  0xb1, 0x03,                     //     FEATURE (Cnst,Var,Abs)
  0x09, 0x23,                     //     USAGE (Intensity)
  0x15, 0x00,                     //     LOGICAL_MINIMUM (0)    
  0x25, 0x7f,                     //     LOGICAL_MAXIMUM (127)
  0x75, 0x08,                     //     REPORT_SIZE (8)
  0x91, 0x02,                     //     OUTPUT (Data,Var,Abs)
  0x09, 0x23,                     //     USAGE (Intensity)
  0xb1, 0x02,                     //     FEATURE (Data,Var,Abs)
  0x09, 0x24,                     //     USAGE (Repeat Count)
  0x91, 0x02,                     //     OUTPUT (Data,Var,Abs)
  0x09, 0x24,                     //     USAGE (Repeat Count)
  0xb1, 0x02,                     //     FEATURE (Data,Var,Abs)
  0x09, 0x25,                     //     USAGE (Retrigger Period)
  0x91, 0x02,                     //     OUTPUT (Data,Var,Abs)
  0x09, 0x25,                     //     USAGE (Retrigger Period)
  0xb1, 0x02,                     //     FEATURE (Data,Var,Abs)
  0x09, 0x28,                     //     USAGE (Waveform Cutoff Time)
  0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32,767)
  0x75, 0x10,                     //     REPORT_SIZE (16)
  0xb1, 0x02,                     //     FEATURE (Data,Var,Abs)
  0x05, 0x0e,                     //     USAGE_PAGE (Haptics)          
  0x09, 0x10,                     //     USAGE (Waveform List)                     
  0xa1, 0x02,                     //     COLLECTION (Logical)
  0x05, 0x0A,                     //       USAGE_PAGE (Ordinal)          
  0x09, 0x03,                     //       USAGE (Ordinal 3)
  0x95, 0x01,                     //       REPORT_COUNT (1)
  0x75, 0x08,                     //       REPORT_SIZE (8)
  0x15, 0x03,                     //       LOGICAL_MINIMUM (3)
  0x25, 0x03,                     //       LOGICAL_MAXIMUM (3)
  0x36, 0x03, 0x10,               //       PHYSICAL_MINIMUM (0x1003)
  0x46, 0x03, 0x10,               //       PHYSICAL_MAXIMUM (0x1003)
  0xb1, 0x03,                     //       FEATURE (Cnst,Var,Abs)
  0x09, 0x04,                     //       USAGE (Ordinal 4)
  0x15, 0x04,                     //       LOGICAL_MINIMUM (4)
  0x25, 0x04,                     //       LOGICAL_MAXIMUM (4)
  0x36, 0x04, 0x10,               //       PHYSICAL_MINIMUM (0x1004)
  0x46, 0x04, 0x10,               //       PHYSICAL_MAXIMUM (0x1004)
  0xb1, 0x03,                     //       FEATURE (Cnst,Var,Abs)
  0xc0,                           //     END_COLLECTION         
  0x05, 0x0e,                     //     USAGE_PAGE (Haptics)          
  0x09, 0x11,                     //     USAGE (Duration List)                     
  0xa1, 0x02,                     //     COLLECTION (Logical)
  0x05, 0x0a,                     //       USAGE_PAGE (Ordinal)          
  0x09, 0x03,                     //       USAGE (Ordinal 3)
  0x09, 0x04,                     //       USAGE (Ordinal 4)
  0x15, 0x00,                     //       LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x0f,               //       LOGICAL_MAXIMUM (4095)
  0x95, 0x02,                     //       REPORT_COUNT (2)
  0x75, 0x10,                     //       REPORT_SIZE (8)
  0xb1, 0x02,                     //       FEATURE (Data,Var,Abs)
  0xc0,                           //     END_COLLECTION
  0xc0,                           //   END_COLLECTION

  0xc0,                           // END_COLLECTION

  

  // ==== Lighting ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  bit   |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                 Report ID (Lighting)                                  |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x01  |              LampUpdateFlags              |                 LampCount                 |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x02  |                                       LampId 0                                        |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  ...   |                                          ...                                          |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x10  |                                       LampId 14                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x11  |                                  RedUpdateChannel 0                                   |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  ...   |                                          ...                                          |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x1f  |                                  RedUpdateChannel 14                                  |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x20  |                                 GreenUpdateChannel 0                                  |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  ...   |                                          ...                                          |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x2e  |                                 GreenUpdateChannel 14                                 |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x2f  |                                  BlueUpdateChannel 0                                  |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  ...   |                                          ...                                          |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x3d  |                                  BlueUpdateChannel 14                                 |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x59,                    // USAGE_PAGE (LightingAndIllumination)
  0x09, 0x01,                    // USAGE (LampArray)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, LIGHTING_REPORT_ID,      //   REPORT_ID (7)
  0x09, 0x20,                    //   USAGE (LampMultiUpdateReport)
  0xa1, 0x02,                    //   COLLECTION (Logical)
  0x09, 0x55,                    //     USAGE (LampUpdateFlags)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
  0x75, 0x04,                    //     REPORT_SIZE (4)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)

  0x09, 0x03,                    //     USAGE (LampCount)
  0x25, 0x0f,                    //     LOGICAL_MAXIMUM (15)
  0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)

  0x09, 0x21,                    //     USAGE (LampId)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x95, 0x0f,                    //     REPORT_COUNT (15)
  0x91, 0x00,                    //     OUTPUT (Data,Ary,Abs)

  0x09, 0x51,                    //     USAGE (RedUpdateChannel)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x91, 0x00,                    //     OUTPUT (Data,Ary,Abs)

  0x09, 0x52,                    //     USAGE (GreenUpdateChannel)
  0x91, 0x00,                    //     OUTPUT (Data,Ary,Abs)

  0x09, 0x53,                    //     USAGE (BlueUpdateChannel)
  0x91, 0x00,                    //     OUTPUT (Data,Ary,Abs)

  0x09, 0x54,                    //     USAGE (IntensityUpdateChannel)
  0x91, 0x00,                    //     OUTPUT (Data,Ary,Abs)

  0xc0,                          //   END_COLLECTION
  0xc0,                          // END_COLLECTION
  
  /* USER CODE END 0 */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

// TODO: Feature reports may not working.
__ALIGN_BEGIN static uint8_t HID_FeatureReport6[] __ALIGN_END = {
  0x06,
  0x00, 0x10,
  0x37, 0x00, 0x01, 0x00,
  0x7f,
  0x7f,
  0x7f,
  0xff, 0x7f,
  0x03,
  0x04,
  0xff, 0x0f,
  0xff, 0x0f,
};

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_Variables USBD_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_HID_Private_FunctionPrototypes USBD_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t HID_Init_FS(void);
static int8_t HID_DeInit_FS(void);
int8_t HID_OutEvent_FS(uint8_t* buffer);
uint8_t* HID_GetReport_FS(uint8_t reportId, uint8_t reportType, uint16_t* length);

/**
  * @}
  */

USBD_HID_ItfTypeDef USBD_HID_fops_FS =
{
  HID_ReportDesc_FS,
  HID_GetReport_FS,
  HID_Init_FS,
  HID_DeInit_FS,
  HID_OutEvent_FS
};

/** @defgroup USBD_HID_Private_Functions USBD_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
__weak int8_t HID_OutEvent_FS(uint8_t* buffer)
{
  /* USER CODE BEGIN 6 */
  
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/

uint8_t* HID_GetReport_FS(uint8_t reportId, uint8_t reportType, uint16_t* length) {
  if (reportId == 6 && reportType == 3) {
    *length = sizeof(HID_FeatureReport6);
    return HID_FeatureReport6;
  }
}

uint8_t USBD_HID_SendCtrlReport_FS(uint16_t ctrl){
  uint8_t ctrlReportBuffer[CTRL_REPORT_LENGTH];
  ctrlReportBuffer[0x00] = CTRL_REPORT_ID;
  ctrlReportBuffer[0x01] = ctrl & 0xFF;
  ctrlReportBuffer[0x02] = ctrl >> 8 & 0xFF;
  return USBD_HID_SendReport(&hUsbDeviceFS, ctrlReportBuffer, CTRL_REPORT_LENGTH);
}

uint8_t USBD_HID_SendKeyboardReport_FS(uint8_t modifier, uint8_t oem, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6) {
  uint8_t keyboardReportBuffer[KEYBOARD_REPORT_LENGTH];
  keyboardReportBuffer[0x00] = KEYBOARD_REPORT_ID;
  keyboardReportBuffer[0x01] = modifier;
  keyboardReportBuffer[0x02] = oem;
  keyboardReportBuffer[0x03] = key1;
  keyboardReportBuffer[0x04] = key2;
  keyboardReportBuffer[0x05] = key3;
  keyboardReportBuffer[0x06] = key4;
  keyboardReportBuffer[0x07] = key5;
  keyboardReportBuffer[0x08] = key6;
  return USBD_HID_SendReport(&hUsbDeviceFS, keyboardReportBuffer, KEYBOARD_REPORT_LENGTH);
}

uint8_t USBD_HID_SendMouseReport_FS(uint8_t buttons, uint8_t x, uint8_t y, uint8_t wheel) {
  uint8_t mouseReportBuffer[MOUSE_REPORT_LENGTH];
  mouseReportBuffer[0x00] = MOUSE_REPORT_ID;
  mouseReportBuffer[0x01] = buttons;
  mouseReportBuffer[0x02] = x;
  mouseReportBuffer[0x03] = y;
  mouseReportBuffer[0x04] = wheel;
  return USBD_HID_SendReport(&hUsbDeviceFS, mouseReportBuffer, MOUSE_REPORT_LENGTH);
}

uint8_t USBD_HID_SendRadialReport_FS(uint8_t button, int16_t dial, uint16_t x, uint16_t y, uint16_t width) {
  uint8_t buffer[RADIAL_REPORT_LENGTH];
  buffer[0x00] = RADIAL_REPORT_ID;
  buffer[0x01] = (button & 0b1) | (dial << 1);
  buffer[0x02] = dial >> 7;
  buffer[0x03] = LOBYTE(x);
  buffer[0x04] = HIBYTE(x);
  buffer[0x05] = LOBYTE(y);
  buffer[0x06] = HIBYTE(y);
  buffer[0x07] = LOBYTE(width);
  buffer[0x08] = HIBYTE(width);
  return USBD_HID_SendReport(&hUsbDeviceFS, buffer, RADIAL_REPORT_LENGTH);
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

