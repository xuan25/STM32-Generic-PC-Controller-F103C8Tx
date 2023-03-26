#ifndef __WHEEL_H
#define __WHEEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "encoder.h"

/**
 * @brief Wheel Structure definition
 * 
 * @note Used to managed a encoder hardware that act like a wheel.
 * Please specify the Encoder struct, TickInterval, ResetDelayMs, IgnoreInputEdge and OnTicked callback.
*/
typedef struct Wheel {
  void (*UserData);               // User data
  Encoder* Encoder;               // Encoder struct
  uint32_t LastInputTickMs;       // Time of the last hardware tick
  int16_t InputState;             // Internal wheel state tracing
  uint16_t TickInterval;          // Specify the ratio between hardware ticks and a wheel tick
  uint16_t ResetDelayMs;          // Specify the time for reset delay that auto align the wheel to a wheel tick
  Encoder_Edge IgnoreInputEdge;   // Specify the pin to ignore its rising/falling edges
  
  /**
   * @brief Tick callback of the wheel
   * 
   * @param sender Wheel that trigger the callback.
   * @param direction Tick direction. Positive for CW. Negative for CCW.
   * @retval None
  */
  void (*OnTicked)(struct Wheel* sender, int8_t direction);
} Wheel;

/**
 * @brief Initialize an wheel
 * 
 * @param wheel The wheel to be initialized
 * @retval None
*/
void Wheel_Init(Wheel* wheel);

/**
 * @brief Scan an wheel to update its state and potentially trigger a callback.
 * 
 * @param wheel The wheel to be scanned
 * @retval None
*/
void Wheel_Scan(Wheel* wheel);

#ifdef __cplusplus
}
#endif

#endif /* __WHEEL_H */
