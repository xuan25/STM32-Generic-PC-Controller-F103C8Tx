/**
  ******************************************************************************
  * @file    usbd_hid.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_hid.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HID_H
#define __USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"
#include  "usbd_composite.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_HID
  * @brief This file is the Header file for USBD_hid.c
  * @{
  */


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */
#ifndef HID_EPIN_ADDR
#define HID_EPIN_ADDR           0x81
#endif /* HID_EPIN_ADDR */

#ifndef HID_EPIN_SIZE
#define HID_EPIN_SIZE           0x40
#endif /* HID_EPIN_SIZE */

#ifndef HID_EPOUT_ADDR
#define HID_EPOUT_ADDR          0x01
#endif /* HID_EPOUT_ADDR */

#ifndef HID_EPOUT_SIZE
#define HID_EPOUT_SIZE          0x40
#endif /* HID_EPOUT_SIZE */

#define USB_HID_CONFIG_DESC_SIZ       41U
#define USB_HID_DESC_SIZ              9U

#ifndef HID_HS_BINTERVAL
#define HID_HS_BINTERVAL            0x05U
#endif /* HID_HS_BINTERVAL */

#ifndef HID_FS_BINTERVAL
#define HID_FS_BINTERVAL            0x05U
#endif /* HID_FS_BINTERVAL */

#ifndef USBD_HID_OUTREPORT_BUF_SIZE
#define USBD_HID_OUTREPORT_BUF_SIZE  0x02U
#endif /* USBD_HID_OUTREPORT_BUF_SIZE */
#ifndef USBD_HID_REPORT_DESC_SIZE
#define USBD_HID_REPORT_DESC_SIZE   163U
#endif /* USBD_HID_REPORT_DESC_SIZE */

#define HID_DESCRIPTOR_TYPE           0x21U
#define HID_REPORT_DESC               0x22U

#define HID_REQ_SET_PROTOCOL          0x0BU
#define HID_REQ_GET_PROTOCOL          0x03U

#define HID_REQ_SET_IDLE              0x0AU
#define HID_REQ_GET_IDLE              0x02U

#define HID_REQ_SET_REPORT            0x09U
#define HID_REQ_GET_REPORT            0x01U
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef enum
{
  HID_IDLE = 0U,
  HID_BUSY,
}
HID_StateTypeDef;

typedef struct _USBD_HID_Itf
{
  uint8_t                  *pReport;
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* OutEvent)(uint8_t event_idx, uint8_t state);

} USBD_HID_ItfTypeDef;

typedef struct
{
  uint8_t              Report_buf[USBD_HID_OUTREPORT_BUF_SIZE];
  uint32_t             Protocol;
  uint32_t             IdleState;
  uint32_t             AltSetting;
  uint32_t             IsReportAvailable;
  HID_StateTypeDef     state;
}
USBD_HID_HandleTypeDef;
/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_HID;
#define USBD_HID_CLASS    &USBD_HID
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t  USBD_HID_Init(USBD_HandleTypeDef *pdev,
                                     uint8_t cfgidx);

uint8_t  USBD_HID_DeInit(USBD_HandleTypeDef *pdev,
                                       uint8_t cfgidx);

uint8_t  USBD_HID_Setup(USBD_HandleTypeDef *pdev,
                                      USBD_SetupReqTypedef *req);

uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev,
                                   uint8_t *report,
                                   uint16_t len);

uint8_t  USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);

uint8_t  USBD_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);

uint8_t  USBD_HID_EP0_RxReady(USBD_HandleTypeDef  *pdev);

uint8_t  USBD_HID_RegisterInterface(USBD_HandleTypeDef   *pdev,
                                           USBD_HID_ItfTypeDef *fops);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_HID_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
