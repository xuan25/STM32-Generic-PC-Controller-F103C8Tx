#ifndef __PUSHABLE_WHEEL_H
#define __PUSHABLE_WHEEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "wheel.h"
#include "binary_push_key.h"

typedef enum PushableWheelLastUpdatedItem {
  ReleasedWheel = 1,
  PressedWheel = 2,
  PushKey = 3,
} PushableWheelLastUpdatedItem;

/**
 * @brief PushableWheel Structure definition
 * 
 * @note Used to managed a pushable wheel.
 * Please specify the ReleasedWheel, PressedWheel, PushKey and callbacks.
*/
typedef struct PushableWheel {
  void (*UserData);                               // User data
  Wheel* ReleasedWheel;                           // Wheel for released mode
  Wheel* PressedWheel;                            // Wheel for pressed mode
  BinaryPushKey* PushKey;                         // PushKey to press/release
  PushableWheelLastUpdatedItem LastUpdatedItem;   // Last updated item 
  
  /**
   * @brief Tick callback of the wheel for released mode
   * 
   * @param sender PushableWheel that trigger the callback.
   * @param direction Tick direction. Positive for CW. Negative for CCW.
   * @retval None
  */
  void (*OnReleasedWheelTicked)(struct PushableWheel* sender, int8_t direction);
  
  /**
   * @brief Tick callback of the wheel for pressed mode
   * 
   * @param sender PushableWheel that trigger the callback.
   * @param direction Tick direction. Positive for CW. Negative for CCW.
   * @retval None
  */
  void (*OnPressedWheelTicked)(struct PushableWheel* sender, int8_t direction);

  /**
   * @brief State changed callback of the key
   * 
   * @param sender PushableWheel that trigger the callback.
   * @param state state
   * @param isWheelTicked Whether the wheel has ticked after the last key state change.
   * @retval None
  */
  void (*OnPushKeyStateChanged)(struct PushableWheel* sender, BinaryPushKeyState state, uint8_t isWheelTicked);
} PushableWheel;

/**
 * @brief Initialize an PushableWheel
 * 
 * @param pushableWheel The wheel to be initialized
 * @retval None
*/
void PushableWheel_Init(PushableWheel* pushableWheel);

/**
 * @brief Scan an PushableWheel to update its state and potentially trigger a callback.
 * 
 * @param pushableWheel The wheel to be scanned
 * @retval None
*/
void PushableWheel_Scan(PushableWheel* pushableWheel);

#ifdef __cplusplus
}
#endif

#endif /* __PUSHABLE_WHEEL_H */
