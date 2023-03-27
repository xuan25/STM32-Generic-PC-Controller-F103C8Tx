#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "gpio_pin.h"

// Debounce time for encoder pins.
#define ENCODER_DEDOUNCE_MS 0u

/**
 * @brief Pin which rising/falling edge occurred.
*/
typedef enum Encoder_Edge
{
  ENCODER_EDGE_A = 1 << 0,
  ENCODER_EDGE_B = 1 << 1,
}Encoder_Edge;

/**
 * @brief Encoder Structure definition
 * 
 * @note Used to manage a encoder hardware.
 * Please specify the GPIO of Pin A and Pin B of the encoder and the OnTicked callback. 
*/
typedef struct Encoder {
  void (*UserData);                   // User data
  GPIO_Pin* PinA;                     // GPIO Pin A
  GPIO_Pin* PinB;                     // GPIO Pin B
  GPIO_PinState OffLevel;             // GPIO pin level for ON state of encoder pin
#if ENCODER_DEDOUNCE_MS > 0
  uint32_t LastLevelChangedMs;        // Time of the last level change on either pin
  uint8_t LastChangedLevelA;          // Last voltage level of Pin A
  uint8_t LastChangedLevelB;          // Last voltage level of Pin B
#endif
  uint8_t LastStateA;                 // Last pin state of Pin A
  uint8_t LastStateB;                 // Last pin state of Pin B

  /**
   * @brief Tick callback of the encoder
   * 
   * @param sender Encoder that trigger the callback.
   * @param direction Tick direction. Positive for CW. Negative for CCW.
   * @param edge Pin which rising/falling edge occurred.
   * @retval None
  */
  void (*OnTicked)(struct Encoder* sender, int8_t direction, Encoder_Edge edge);
} Encoder;

/**
 * @brief Initialize an encoder
 * 
 * @param encoder The encoder to be initialized
 * @retval None
*/
void Encoder_Init(Encoder* encoder);

/**
 * @brief Scan an encoder hardware to update its state and potentially trigger a callback.
 * 
 * @param encoder The encoder to be scanned
 * @retval None
*/
void Encoder_Scan(Encoder* encoder);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
