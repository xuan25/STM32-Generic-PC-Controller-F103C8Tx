#include "keymat.h"
#include <stdlib.h>

void MatKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState);

void Keymat_Init(KeyMat* keyMat) {

  uint16_t numItemsMax = keyMat->NumRows * keyMat->NumCols;

  // Init lookup
  keyMat->Internal.Keys = (Key**) malloc(numItemsMax * sizeof(Key*));
  keyMat->Internal.EnabledFlags = (uint8_t*) malloc(numItemsMax * sizeof(uint8_t));
  for (int i = 0; i < numItemsMax; i++) {
    keyMat->Internal.Keys[i] = NULL;
    keyMat->Internal.EnabledFlags[i] = 0;
  }

  // Init relations
  for (int i = 0; i < keyMat->NumMatKeys; i++) {
    MatKey* matKey = keyMat->MatKeys[i];
    matKey->Internal.Parent = keyMat;
    matKey->Key->Internal.Parent = matKey;
    matKey->Key->OnStateChanged = MatKey_OnKeyStateChanged;

    uint8_t idx = matKey->Y * keyMat->NumCols + matKey->X;
    keyMat->Internal.Keys[idx] = matKey->Key;
    keyMat->Internal.EnabledFlags[idx] = 1;
  }
  
  // Init state
  uint32_t tickMs = HAL_GetTick();
  for (int x = 0; x < keyMat->NumRows; x++) {
    GPIO_Pin* row = keyMat->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, keyMat->ReleasedLevel);
  }
  for (int x = 0; x < keyMat->NumRows; x++) {
    GPIO_Pin* row = keyMat->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, ~keyMat->ReleasedLevel & 0b1);
    for (int y = 0; y < keyMat->NumCols; y++) {
      uint8_t idx = x * keyMat->NumCols + y;
      if (!keyMat->Internal.EnabledFlags[idx]) {
        continue;
      }
      Key* key = keyMat->Internal.Keys[idx];

      GPIO_Pin* col = keyMat->Cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      key->Internal.LastChangedLevel = level;
      key->Internal.LastLevelChangedMs = tickMs;
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, keyMat->ReleasedLevel);
  }
}

void Keymat_Scan(KeyMat* keyMat) {
  for (int x = 0; x < keyMat->NumRows; x++) {
    GPIO_Pin* row = keyMat->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, ~keyMat->ReleasedLevel & 0b1);
    for (int y = 0; y < keyMat->NumCols; y++) {
      uint8_t idx = x * keyMat->NumCols + y;
      if (!keyMat->Internal.EnabledFlags[idx]) {
        continue;
      }
      Key* key = keyMat->Internal.Keys[idx];

      GPIO_Pin* col = keyMat->Cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      Key_Update(key, level);
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, keyMat->ReleasedLevel);
  }
}

void MatKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState) {
  MatKey* matKey = (MatKey*)sender->Internal.Parent;
  KeyMat* keyMat = (KeyMat*)matKey->Internal.Parent;

  BinaryPushKeyState state;
  if (newState == keyMat->ReleasedLevel) {
    state = PushKeyReleased;
  } else {
    state = PushKeyPressed;
  }
  matKey->OnStateChanged(matKey, state);
}