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