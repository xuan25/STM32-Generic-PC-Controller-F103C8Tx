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
    ActionConfig* a = actionConfigs;

    uint32_t* pData = ((uint32_t*)actionConfigs) + (i / 4);
    if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ACTION_CONFIG_ADDR + i, *pData) == HAL_OK)
    {
      return HAL_FLASH_GetError();
    }
  }
  for (uint32_t i = 0; i < RGB_CONFIG_SIZE; i += 4) {
    uint32_t* pData = ((uint32_t*)rgbIndex) + (i / 4);
    if (!HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, RGB_CONFIG_ADDR + i, *pData) == HAL_OK)
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
      uint32_t* pData = ((uint32_t*)actionConfigs) + (i / 4);
      *pData = *(__IO uint32_t *)(ACTION_CONFIG_ADDR + i);
    }
    for (uint32_t i = 0; i < RGB_CONFIG_SIZE; i += 4) {
      uint32_t* pData = ((uint32_t*)rgbIndex) + (i / 4);
      *pData = *(__IO uint32_t *)(RGB_CONFIG_ADDR + i);
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
