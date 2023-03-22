#include "key.h"

void GPIOKey_Init(GPIOKey* gpio_key) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(gpio_key->GPIOx, gpio_key->GPIO_Pin);
  gpio_key->Key->LastChangedLevel = keyLevel;
#if KEY_DE_JITTERING_MS > 0
  uint32_t tickMs = HAL_GetTick();
  gpio_key->Key->LastLevelChangedMs = tickMs;
#endif
}

void GPIOKey_Scan(GPIOKey* gpio_key) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(gpio_key->GPIOx, gpio_key->GPIO_Pin);
  Key_Update(gpio_key->Key, keyLevel);
}

void Key_Update(Key* key, uint8_t level) {
#if KEY_DE_JITTERING_MS > 0
  uint32_t tickMs = HAL_GetTick();
  // Level update
  if(key->LastChangedLevel != level) {
    key->LastChangedLevel = level;
    key->LastLevelChangedMs = tickMs;
  }
  // State update
  if(key->State != level && tickMs - key->LastLevelChangedMs > KEY_DE_JITTERING_MS) {
    uint8_t oldState = key->State;
    key->State = level;
    (*key->OnStateChanged)(key, oldState, level);
  }
#else
  // State update
  if(key->State != level) {
    uint8_t oldState = key->State;
    key->State = level;
    (*key->OnStateChanged)(key, oldState, level);
  }
#endif
}
