#ifndef __WHEEL_H
#define __WHEEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "encoder.h"

typedef struct Wheel {
  void (*UserData);
  Encoder* Encoder;
  uint32_t LastInputTick;
  int16_t InputState;
  uint16_t TickInterval;
  uint16_t ResetDelay;
  uint8_t IgnoreInputEdge;
  void (*OnTicked)(struct Wheel* sender, int8_t direction);
} Wheel;


void Wheel_Init(Wheel* wheel);
void Wheel_Scan(Wheel* wheel);

#ifdef __cplusplus
}
#endif

#endif /* __WHEEL_H */
