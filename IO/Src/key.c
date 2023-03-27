#include "key.h"

void GPIOKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState);

void GPIOKey_Init(GPIOKey* gpioKey) {
  gpioKey->Key->UserData = gpioKey;

  gpioKey->Key->OnStateChanged = GPIOKey_OnKeyStateChanged;

  uint8_t keyLevel = HAL_GPIO_ReadPin(gpioKey->Pin->GPIOx, gpioKey->Pin->GPIO_Pin);
  Key_Init(gpioKey->Key, keyLevel);
}

void GPIOKey_Scan(GPIOKey* gpioKey) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(gpioKey->Pin->GPIOx, gpioKey->Pin->GPIO_Pin);
  Key_Update(gpioKey->Key, keyLevel);
}

void GPIOKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState) {
  GPIOKey* gpioKey = (GPIOKey*)sender->UserData;
  BinaryKeyState state;
  if (newState == gpioKey->ReleasedLevel) {
    state = Released;
  } else {
    state = Pressed;
  }
  gpioKey->OnStateChanged(gpioKey, state);
}

void Key_Init(Key* key, uint8_t level) {
  key->LastChangedLevel = level;
  #if KEY_DEBOUNCE_MS > 0
    uint32_t tickMs = HAL_GetTick();
    key->LastLevelChangedMs = tickMs;
  #endif
}

void Key_Update(Key* key, uint8_t level) {
#if KEY_DEBOUNCE_MS > 0
  uint32_t tickMs = HAL_GetTick();
  // Level update
  if(key->LastChangedLevel != level) {
    key->LastChangedLevel = level;
    key->LastLevelChangedMs = tickMs;
  }
  // State update
  if(key->State != level && tickMs - key->LastLevelChangedMs > KEY_DEBOUNCE_MS) {
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
