#ifndef __KEYMAT_H
#define __KEYMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "gpio_pin.h"
#include "key.h"

/**
 * @brief MatKey Structure definition
 * 
 * @note Used to managed a key hardware in a key matrix.
*/
typedef struct MatKey {
  void (*UserData);   // User data
  Key (*Key);         // Key structure
  uint16_t X;         // X coordinate of the key
  uint16_t Y;         // Y coordinate of the key

  /**
   * @brief State changed callback of the MatKey
   * 
   * @param sender MatKey that trigger the callback.
   * @param state state.
   * @retval None
  */
  void (*OnStateChanged)(struct MatKey* sender, BinaryPushKeyState state);
} MatKey;

/**
 * @brief KeyMat Structure definition
 * 
 * @note Used to managed a key matrix.
 * Please specify NumMatKeys, MatKeys, NumRows, Rows, NumCols and Cols.
*/
typedef struct KeyMat {
  void (*UserData);             // User data
  uint16_t NumMatKeys;          // Number of defined MatKey
  MatKey* (*MatKeys);           // A array of defined MatKey
  uint16_t NumRows;             // Number of row pins
  GPIO_Pin* (*Rows);            // A array of row pins
  uint16_t NumCols;             // Number of column pins
  GPIO_Pin* (*Cols);            // A array of column pins
  GPIO_PinState ReleasedLevel;  // GPIO state when key released
  Key* (* Keys);                // Keys for matrix coordinates
  uint8_t* EnabledFlags;        // Enabled flags for matrix coordinates
} KeyMat;

/**
 * @brief Initialize the Key matrix
 * 
 * @param keyMat The KeyMat to be initialized
 * @retval None
*/
void Keymat_Init(KeyMat* keyMat);

/**
 * @brief Scan an Key matrix hardware to update its state and potentially 
 * trigger callbacks.
 * 
 * @param keyMat The KeyMat to be scanned
 * @retval None
*/
void Keymat_Scan(KeyMat* keyMat);

#ifdef __cplusplus
}
#endif

#endif /* __KEYMAT_H */
