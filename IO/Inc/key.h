#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "gpio_pin.h"

// Debounce time for key pins.
#define KEY_DEBOUNCE_MS 10u

typedef struct Key_Internal {
  void (*Parent);                   // Parent
  uint8_t State;                    // State for the key
#if KEY_DEBOUNCE_MS > 0
  uint32_t LastLevelChangedMs;      // Time of the last level change
  uint8_t LastChangedLevel;         // Last voltage level
#endif
} Key_Internal;

/**
 * @brief Key Structure definition
 * 
 * @note Used to manage a key (any type) hardware.
*/
typedef struct Key {
  Key_Internal Internal;            // For internal usage

  /**
   * @brief State changed callback of the key
   * 
   * @param sender Key that trigger the callback.
   * @param oldState Old state.
   * @param newState New state.
   * @retval None
  */
  void (*OnStateChanged)(struct Key* sender, uint8_t oldState, uint8_t newState);
} Key;

/**
 * @brief Initialize a BinaryPushKey
 * 
 * @param key: The key to be initialize
 * @param level: Level of the key
 * @retval None
*/
void Key_Init(Key* key, uint8_t level);

/**
 * @brief Update a Key with a new voltage level
 * 
 * @param key: The key to be updated
 * @param level: Level of the key
 * @retval None
*/
void Key_Update(Key* key, uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
