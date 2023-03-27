#include "key.h"

void BinaryPushKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState);

void BinaryPushKey_Init(BinaryPushKey* binaryPushKey) {
  binaryPushKey->Key->UserData = binaryPushKey;

  binaryPushKey->Key->OnStateChanged = BinaryPushKey_OnKeyStateChanged;

  uint8_t keyLevel = HAL_GPIO_ReadPin(binaryPushKey->Pin->GPIOx, binaryPushKey->Pin->GPIO_Pin);
  Key_Init(binaryPushKey->Key, keyLevel);
}

void BinaryPushKey_Scan(BinaryPushKey* binaryPushKey) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(binaryPushKey->Pin->GPIOx, binaryPushKey->Pin->GPIO_Pin);
  Key_Update(binaryPushKey->Key, keyLevel);
}

void BinaryPushKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState) {
  BinaryPushKey* binaryPushKey = (BinaryPushKey*)sender->UserData;
  BinaryPushKeyState state;
  if (newState == binaryPushKey->ReleasedLevel) {
    state = PushKeyReleased;
  } else {
    state = PushKeyPressed;
  }
  binaryPushKey->OnStateChanged(binaryPushKey, state);
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
