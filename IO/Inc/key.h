#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define KEY_DE_JITTERING_MS 10u

typedef struct Key {
  uint8_t State;
#if KEY_DE_JITTERING_MS > 0
  uint32_t LastLevelChangedMs;
  uint8_t LastChangedLevel;
#endif
  void (*OnStateChanged)(struct Key* sender, uint8_t oldState, uint8_t newState);
} Key;

typedef struct GPIOKey {
  Key (*Key);
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
} GPIOKey;

void Key_Init(GPIOKey* gpio_key);
void Key_Scan(GPIOKey* gpio_key);
void Key_Update(Key* key, uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
