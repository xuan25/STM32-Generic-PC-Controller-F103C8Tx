#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "gpio_pin.h"

// Debounce time for key pins.
#define KEY_DEBOUNCE_MS 10u

/**
 * @brief Key Structure definition
 * 
 * @note If a key hardware need to be managed, 
 * please use BinaryPushKey or MatKey instead and specify the OnStateChanged callback here.
*/
typedef struct Key {
  void (*UserData);                 // User data
  uint8_t State;                    // State for the key
#if KEY_DEBOUNCE_MS > 0
  uint32_t LastLevelChangedMs;      // Time of the last level change
  uint8_t LastChangedLevel;         // Last voltage level
#endif

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
