#ifndef __PUSHABLE_WHEEL_H
#define __PUSHABLE_WHEEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "wheel.h"
#include "key.h"

typedef enum PushableWheelLastUpdatedItem {
  ReleasedWheel = 1,
  PressedWheel = 2,
  PushKey = 3,
} PushableWheelLastUpdatedItem;

/**
 * @brief PushableWheel Structure definition
 * 
 * @note Used to managed a encoder hardware that act like a wheel.
 * Please specify the Encoder struct, TickInterval, ResetDelayMs, IgnoreInputEdge and OnTicked callback.
*/
typedef struct PushableWheel {
  void (*UserData);               // User data
  Wheel* ReleasedWheel;                 // 
  Wheel* PressedWheel;                 // 
  GPIOKey* PushKey;               // 
  PushableWheelLastUpdatedItem LastUpdatedItem;
  
  // /**
  //  * @brief Tick callback of the wheel
  //  * 
  //  * @param sender Wheel that trigger the callback.
  //  * @param direction Tick direction. Positive for CW. Negative for CCW.
  //  * @retval None
  // */
  void (*OnReleasedWheelTicked)(struct PushableWheel* sender, int8_t direction);
  void (*OnPressedWheelTicked)(struct PushableWheel* sender, int8_t direction);
  void (*OnPushKeyStateChanged)(struct PushableWheel* sender, BinaryKeyState state, uint8_t isWheelTicked);
} PushableWheel;

/**
 * @brief Initialize an wheel
 * 
 * @param wheel The wheel to be initialized
 * @retval None
*/
void PushableWheel_Init(PushableWheel* pushableWheel);

/**
 * @brief Scan an wheel to update its state and potentially trigger a callback.
 * 
 * @param wheel The wheel to be scanned
 * @retval None
*/
void PushableWheel_Scan(PushableWheel* pushableWheel);

#ifdef __cplusplus
}
#endif

#endif /* __PUSHABLE_WHEEL_H */
