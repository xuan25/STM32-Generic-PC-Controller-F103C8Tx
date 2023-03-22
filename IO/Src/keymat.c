#include "keymat.h"

GPIO_Pin* keymat_rows;
GPIO_Pin* keymat_cols;

Key* keymat_keys[KEYMAT_ROWS * KEYMAT_ROWS];
uint8_t keymat_enabled_flags[KEYMAT_ROWS * KEYMAT_ROWS];

void Keymat_Init(GPIO_Pin rows[], GPIO_Pin cols[], MatKey* matkeys[], uint8_t num_keys) {
  keymat_rows = rows;
  keymat_cols = cols;

  for (int i = 0; i < num_keys; i++) {
    MatKey* matkey = matkeys[i];
    uint8_t idx = matkey->Y * KEYMAT_COLS + matkey->X;
    keymat_keys[idx] = matkey->Key;
    keymat_enabled_flags[idx] = 1;
  }
  
  // Init states
  uint32_t tickMs = HAL_GetTick();
  for (int x = 0; x < KEYMAT_ROWS; x++) {
    GPIO_Pin* row = &keymat_rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_RESET);
  }
  for (int x = 0; x < KEYMAT_ROWS; x++) {
    GPIO_Pin* row = &keymat_rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_SET);
    for (int y = 0; y < KEYMAT_COLS; y++) {
      uint8_t idx = x * KEYMAT_COLS + y;
      if (!keymat_enabled_flags[idx]) {
        continue;
      }
      Key* key = keymat_keys[idx];

      GPIO_Pin* col = &keymat_cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      key->LastChangedLevel = level;
      key->LastLevelChangedMs = tickMs;
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_RESET);
  }
}

void Keymat_Scan() {
  for (int x = 0; x < KEYMAT_ROWS; x++) {
    GPIO_Pin* row = &keymat_rows[x];
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_SET);
    for (int y = 0; y < KEYMAT_COLS; y++) {
      uint8_t idx = x * KEYMAT_COLS + y;
      if (!keymat_enabled_flags[idx]) {
        continue;
      }
      Key* key = keymat_keys[idx];

      GPIO_Pin* col = &keymat_cols[y];
      GPIO_PinState level = HAL_GPIO_ReadPin(col->GPIOx, col->GPIO_Pin);

      Key_Update(key, level);
    }
    HAL_GPIO_WritePin(row->GPIOx, row->GPIO_Pin, GPIO_PIN_RESET);
  }
}