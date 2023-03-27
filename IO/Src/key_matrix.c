#include "key_matrix.h"
#include <stdlib.h>

void MatrixKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState);

void KeyMatrix_Init(KeyMatrix* keyMatrix) {

  uint16_t numItemsMax = keyMatrix->NumRows * keyMatrix->NumCols;

  // Init lookup
  keyMatrix->Internal.Keys = (Key**) malloc(numItemsMax * sizeof(Key*));
  keyMatrix->Internal.EnabledFlags = (uint8_t*) malloc(numItemsMax * sizeof(uint8_t));
  for (int i = 0; i < numItemsMax; i++) {
    keyMatrix->Internal.Keys[i] = NULL;
    keyMatrix->Internal.EnabledFlags[i] = 0;
  }

  // Init relations
  for (int i = 0; i < keyMatrix->NumMatrixKeys; i++) {
    MatrixKey* matrixKey = keyMatrix->MatrixKeys[i];
    matrixKey->Internal.Parent = keyMatrix;
    matrixKey->Key->Internal.Parent = matrixKey;
    matrixKey->Key->OnStateChanged = MatrixKey_OnKeyStateChanged;

    uint8_t idx = matrixKey->Y * keyMatrix->NumCols + matrixKey->X;
    keyMatrix->Internal.Keys[idx] = matrixKey->Key;
    keyMatrix->Internal.EnabledFlags[idx] = 1;
  }
  
  // Init state
  uint32_t tickMs = HAL_GetTick();
  for (int x = 0; x < keyMatrix->NumRows; x++) {
    GPIO_Pin* row = keyMatrix->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, keyMatrix->ReleasedLevel);
  }
  for (int x = 0; x < keyMatrix->NumRows; x++) {
    GPIO_Pin* row = keyMatrix->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, ~keyMatrix->ReleasedLevel & 0b1);
    for (int y = 0; y < keyMatrix->NumCols; y++) {
      uint8_t idx = x * keyMatrix->NumCols + y;
      if (!keyMatrix->Internal.EnabledFlags[idx]) {
        continue;
      }
      Key* key = keyMatrix->Internal.Keys[idx];

      GPIO_Pin* col = keyMatrix->Cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      key->Internal.LastChangedLevel = level;
      key->Internal.LastLevelChangedMs = tickMs;
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, keyMatrix->ReleasedLevel);
  }
}

void KeyMatrix_DeInit(KeyMatrix* keyMatrix) {
  free(keyMatrix->Internal.Keys);
  free(keyMatrix->Internal.EnabledFlags);
}

void KeyMatrix_Scan(KeyMatrix* keyMatrix) {
  for (int x = 0; x < keyMatrix->NumRows; x++) {
    GPIO_Pin* row = keyMatrix->Rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, ~keyMatrix->ReleasedLevel & 0b1);
    for (int y = 0; y < keyMatrix->NumCols; y++) {
      uint8_t idx = x * keyMatrix->NumCols + y;
      if (!keyMatrix->Internal.EnabledFlags[idx]) {
        continue;
      }
      Key* key = keyMatrix->Internal.Keys[idx];

      GPIO_Pin* col = keyMatrix->Cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      Key_Update(key, level);
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, keyMatrix->ReleasedLevel);
  }
}

void MatrixKey_OnKeyStateChanged(Key *sender, uint8_t oldState, uint8_t newState) {
  MatrixKey* matrixKey = (MatrixKey*)sender->Internal.Parent;
  KeyMatrix* keyMatrix = (KeyMatrix*)matrixKey->Internal.Parent;

  BinaryPushKeyState state;
  if (newState == keyMatrix->ReleasedLevel) {
    state = PushKeyReleased;
  } else {
    state = PushKeyPressed;
  }
  matrixKey->OnStateChanged(matrixKey, state);
}