#ifndef __KEY_MATRIX_H
#define __KEY_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "gpio_pin.h"
#include "binary_push_key.h"

typedef struct MatKey_Internal {
  void (*Parent);     // Parent
} MatKey_Internal;

/**
 * @brief MatrixKey Structure definition
 * 
 * @note Used to managed a key hardware in a key matrix.
*/
typedef struct MatrixKey {
  MatKey_Internal Internal;
  Key (*Key);         // Key structure
  uint16_t X;         // X coordinate of the key
  uint16_t Y;         // Y coordinate of the key

  /**
   * @brief State changed callback of the MatrixKey
   * 
   * @param sender MatrixKey that trigger the callback.
   * @param state state.
   * @retval None
  */
  void (*OnStateChanged)(struct MatrixKey* sender, BinaryPushKeyState state);
} MatrixKey;

typedef struct KeyMat_Internal {
  void (*Parent);               // Parent
  Key* (* Keys);                // Keys for matrix coordinates
  uint8_t* EnabledFlags;        // Enabled flags for matrix coordinates
} KeyMat_Internal;

/**
 * @brief KeyMatrix Structure definition
 * 
 * @note Used to managed a key matrix.
*/
typedef struct KeyMatrix {
  KeyMat_Internal Internal;     // For internal usage
  uint16_t NumMatrixKeys;       // Number of defined MatrixKey
  MatrixKey* (*MatrixKeys);     // A array of defined MatrixKey
  uint16_t NumRows;             // Number of row pins
  GPIO_Pin* (*Rows);            // A array of row pins
  uint16_t NumCols;             // Number of column pins
  GPIO_Pin* (*Cols);            // A array of column pins
  GPIO_PinState ReleasedLevel;  // GPIO state when key released
} KeyMatrix;

/**
 * @brief Initialize the Key matrix
 * 
 * @param keyMatrix The KeyMatrix to be initialized
 * @retval None
*/
void KeyMatrix_Init(KeyMatrix* keyMatrix);

/**
 * @brief De-initialize the Key matrix
 * 
 * @param keyMatrix The KeyMatrix to be de-initialize
 * @retval None
*/
void KeyMatrix_DeInit(KeyMatrix* keyMatrix);

/**
 * @brief Scan an Key matrix hardware to update its state and potentially 
 * trigger callbacks.
 * 
 * @param keyMatrix The KeyMatrix to be scanned
 * @retval None
*/
void KeyMatrix_Scan(KeyMatrix* keyMatrix);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_MATRIX_H */
