#ifndef __KEYMAT_H
#define __KEYMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
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
} MatKey;

// Number of rows of the key matrix
#define KEYMAT_ROWS 4u
// Number of columns of the key matrix
#define KEYMAT_COLS 4u

/**
 * @brief GPIO_Pin Structure definition
 * 
 * @note Used to define pins of the matrix
*/
typedef struct GPIO_Pin {
  GPIO_TypeDef* GPIOx;    // Pin
  uint16_t GPIO_Pin;      // Pin
} GPIO_Pin;

/**
 * @brief Initialize the Key matrix
 * 
 * @param rows: An array of GPIO_Pin for row pins
 * @param cols: An array of GPIO_Pin for column pins
 * @param matkeys: An array of MatKey pointers to specify the keys in the matrix
 * @param numKeys: Number of keys in the matkeys
 * @retval None
*/
void Keymat_Init(GPIO_Pin rows[], GPIO_Pin cols[], MatKey* matkeys[], uint8_t numKeys);

/**
 * @brief Scan an Key matrix hardware to update its state and potentially 
 * trigger callbacks.
 * 
 * @retval None
*/
void Keymat_Scan();

#ifdef __cplusplus
}
#endif

#endif /* __KEYMAT_H */
