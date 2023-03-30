#include "binary_push_key.h"

void BinaryPushKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState);

void BinaryPushKey_Init(BinaryPushKey* binaryPushKey) {
  binaryPushKey->Key->Internal.Parent = binaryPushKey;
  binaryPushKey->Key->Internal.OnStateChanged = BinaryPushKey_OnKeyStateChanged;

  uint8_t keyLevel = HAL_GPIO_ReadPin(binaryPushKey->Pin->GPIOx, binaryPushKey->Pin->GPIO_Pin);
  Key_Init(binaryPushKey->Key, keyLevel);
}

void BinaryPushKey_Scan(BinaryPushKey* binaryPushKey) {
  uint8_t keyLevel = HAL_GPIO_ReadPin(binaryPushKey->Pin->GPIOx, binaryPushKey->Pin->GPIO_Pin);
  Key_Update(binaryPushKey->Key, keyLevel);
}

void BinaryPushKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState) {
  BinaryPushKey* binaryPushKey = (BinaryPushKey*)sender->Internal.Parent;
  BinaryPushKeyState state;
  if (newState == binaryPushKey->ReleasedLevel) {
    state = PushKeyReleased;
  } else {
    state = PushKeyPressed;
  }
  if(binaryPushKey->OnStateChanged == NULL || !binaryPushKey->OnStateChanged(binaryPushKey, state)){
    if(binaryPushKey->Internal.OnStateChanged != NULL) {
      binaryPushKey->Internal.OnStateChanged(binaryPushKey, state);
    }
  }
}
