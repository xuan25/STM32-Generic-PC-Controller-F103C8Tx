#include "keymat.h"
#include <stdlib.h>

void MatKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState);

void Keymat_Init(KeyMat* keyMat) {

  uint16_t numItemsMax = keyMat->NumRows * keyMat->NumCols;

  // Init lookup
  keyMat->Keys = (Key**) malloc(numItemsMax * sizeof(Key*));
  keyMat->EnabledFlags = (uint8_t*) malloc(numItemsMax * sizeof(uint8_t));
  for (int i = 0; i < numItemsMax; i++) {
    keyMat->Keys[i] = NULL;
    keyMat->EnabledFlags[i] = 0;
  }

  // Init relations
  for (int i = 0; i < keyMat->NumMatKeys; i++) {
    MatKey* matKey = keyMat->MatKeys[i];
    matKey->UserData = keyMat;
    matKey->Key->UserData = matKey;
    matKey->Key->OnStateChanged = MatKey_OnKeyStateChanged;

    uint8_t idx = matKey->Y * keyMat->NumCols + matKey->X;
    keyMat->Keys[idx] = matKey->Key;
    keyMat->EnabledFlags[idx] = 1;
  }
  
  // Init state
  uint32_t tickMs = HAL_GetTick();
  for (int x = 0; x < keyMat->NumRows; x++) {
    GPIO_Pin* row = keyMat->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_RESET);
  }
  for (int x = 0; x < keyMat->NumRows; x++) {
    GPIO_Pin* row = keyMat->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_SET);
    for (int y = 0; y < keyMat->NumCols; y++) {
      uint8_t idx = x * keyMat->NumCols + y;
      if (!keyMat->EnabledFlags[idx]) {
        continue;
      }
      Key* key = keyMat->Keys[idx];

      GPIO_Pin* col = keyMat->Cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      key->LastChangedLevel = level;
      key->LastLevelChangedMs = tickMs;
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_RESET);
  }
}

void Keymat_Scan(KeyMat* keyMat) {
  for (int x = 0; x < keyMat->NumRows; x++) {
    GPIO_Pin* row = keyMat->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_SET);
    for (int y = 0; y < keyMat->NumCols; y++) {
      uint8_t idx = x * keyMat->NumCols + y;
      if (!keyMat->EnabledFlags[idx]) {
        continue;
      }
      Key* key = keyMat->Keys[idx];

      GPIO_Pin* col = keyMat->Cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      Key_Update(key, level);
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_RESET);
  }
}

void MatKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState) {
  MatKey* matKey = (MatKey*)sender->UserData;
  KeyMat* keyMat = (KeyMat*)matKey->UserData;

  BinaryPushKeyState state;
  if (newState == keyMat->ReleasedLevel) {
    state = PushKeyReleased;
  } else {
    state = PushKeyPressed;
  }
  matKey->OnStateChanged(matKey, state);
}