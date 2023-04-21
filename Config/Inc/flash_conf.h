/**
  ******************************************************************************
  * @file    flash_conf.h
  * @brief   
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

#ifndef __FLASH_CONF_H
#define __FLASH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

uint32_t FlashConfig_Save();
uint32_t FlashConfig_Load();
uint32_t FlashConfig_Reset();

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_CONF_H */