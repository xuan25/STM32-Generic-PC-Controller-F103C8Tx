/**
  ******************************************************************************
  * @file    flash_conf.c
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

#include "flash_conf.h"

#include "inputs_conf.h"
#include "lighting_conf.h"

#define CONFIG_PAGE_ADDR      0x0801FC00
#define CONFIG_FLAG_ADDR      CONFIG_PAGE_ADDR
#define CONFIG_FLAG_SIZE      sizeof(uint32_t)
#define ACTION_CONFIG_ADDR    CONFIG_FLAG_ADDR + CONFIG_FLAG_SIZE
#define ACTION_CONFIG_SIZE    sizeof(actionConfigs)
#define RGB_CONFIG_ADDR       ACTION_CONFIG_ADDR + ACTION_CONFIG_SIZE 
#define RGB_CONFIG_SIZE       sizeof(rgbIndex)
#define DIAL_LIGHT_MAPPING_CONFIG_ADDR         RGB_CONFIG_ADDR + RGB_CONFIG_SIZE 
#define DIAL_LIGHT_MAPPING_CONFIG_SIZE         sizeof(dialLightMapping)
#define LIGHT_MIDICC_MAPPING_CONFIG_ADDR       DIAL_LIGHT_MAPPING_CONFIG_ADDR + DIAL_LIGHT_MAPPING_CONFIG_SIZE 
#define LIGHT_MIDICC_MAPPING_CONFIG_SIZE       sizeof(lightMIDICCMappings)


uint32_t FlashConfig_Save() {
	uint32_t pageError;

  HAL_FLASH_Unlock();

  static FLASH_EraseInitTypeDef eraseInitStruct = {
    .TypeErase = FLASH_TYPEERASE_PAGES,
    .PageAddress = CONFIG_PAGE_ADDR,
    .NbPages = 1
  };

  if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
  {
    /*Error occurred while page erase.*/
    return HAL_FLASH_GetError();
  }

  /* Program the user Flash area word by word*/
  for (uint32_t i = 0; i < ACTION_CONFIG_SIZE; i += 4) {
    uint32_t *pData = ((uint32_t *)actionConfigs) + (i / 4);
    if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ACTION_CONFIG_ADDR + i, *pData) == HAL_OK)
    {
      return HAL_FLASH_GetError();
    }
  }
  for (uint32_t i = 0; i < RGB_CONFIG_SIZE; i += 4) {
    uint32_t *pData = ((uint32_t *)rgbIndex) + (i / 4);
    if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, RGB_CONFIG_ADDR + i, *pData) == HAL_OK)
    {
      return HAL_FLASH_GetError();
    }
  }
  for (uint32_t i = 0; i < DIAL_LIGHT_MAPPING_CONFIG_SIZE; i += 4) {
    uint32_t *pData = ((uint32_t *)dialLightMapping) + (i / 4);
    if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, DIAL_LIGHT_MAPPING_CONFIG_ADDR + i, *pData) == HAL_OK)
    {
      return HAL_FLASH_GetError();
    }
  }
  for (uint32_t i = 0; i < LIGHT_MIDICC_MAPPING_CONFIG_SIZE; i += 4) {
    uint32_t *pData = ((uint32_t *)lightMIDICCMappings) + (i / 4);
    if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, LIGHT_MIDICC_MAPPING_CONFIG_ADDR + i, *pData) == HAL_OK)
    {
      return HAL_FLASH_GetError();
    }
  }
  if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, CONFIG_FLAG_ADDR, 1) == HAL_OK)
  {
    return HAL_FLASH_GetError();
  }

  HAL_FLASH_Lock();

  return 0;
}

uint32_t FlashConfig_Load() {
  uint32_t flag = *(__IO uint32_t *)CONFIG_FLAG_ADDR;

  if(flag == 1) {
    for (uint32_t i = 0; i < ACTION_CONFIG_SIZE; i += 4) {
      uint32_t *pData = ((uint32_t*)actionConfigs) + (i / 4);
      *pData = *(__IO uint32_t *)(ACTION_CONFIG_ADDR + i);
    }
    for (uint32_t i = 0; i < RGB_CONFIG_SIZE; i += 4) {
      uint32_t *pData = ((uint32_t*)rgbIndex) + (i / 4);
      *pData = *(__IO uint32_t *)(RGB_CONFIG_ADDR + i);
    }
    for (uint32_t i = 0; i < DIAL_LIGHT_MAPPING_CONFIG_SIZE; i += 4) {
      uint32_t *pData = ((uint32_t*)dialLightMapping) + (i / 4);
      *pData = *(__IO uint32_t *)(DIAL_LIGHT_MAPPING_CONFIG_ADDR + i);
    }
    for (uint32_t i = 0; i < LIGHT_MIDICC_MAPPING_CONFIG_SIZE; i += 4) {
      uint32_t *pData = ((uint32_t*)lightMIDICCMappings) + (i / 4);
      *pData = *(__IO uint32_t *)(LIGHT_MIDICC_MAPPING_CONFIG_ADDR + i);
    }
  }

  return 0;
}

uint32_t FlashConfig_Reset() {
	uint32_t pageError;

  HAL_FLASH_Unlock();

  static FLASH_EraseInitTypeDef eraseInitStruct = {
    .TypeErase = FLASH_TYPEERASE_PAGES,
    .PageAddress = CONFIG_PAGE_ADDR,
    .NbPages = 1
  };

  if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
  {
    /*Error occurred while page erase.*/
    return HAL_FLASH_GetError();
  }

  HAL_FLASH_Lock();

  return 0;
}
