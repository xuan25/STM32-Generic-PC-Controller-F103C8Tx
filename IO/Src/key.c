#include "key.h"

void GPIOKey_Init(GPIOKey* gpioKey) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(gpioKey->Pin->GPIOx, gpioKey->Pin->GPIO_Pin);
  gpioKey->Key->LastChangedLevel = keyLevel;
#if KEY_DE_JITTERING_MS > 0
  uint32_t tickMs = HAL_GetTick();
  gpioKey->Key->LastLevelChangedMs = tickMs;
#endif
}

void GPIOKey_Scan(GPIOKey* gpioKey) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(gpioKey->Pin->GPIOx, gpioKey->Pin->GPIO_Pin);
  Key_Update(gpioKey->Key, keyLevel);
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
