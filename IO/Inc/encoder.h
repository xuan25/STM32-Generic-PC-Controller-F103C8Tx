#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define ENCODER_DE_JITTERING_MS 0u

typedef enum Encoder_Edge
{
  ENCODER_EDGE_A = 1 << 0,
  ENCODER_EDGE_B = 1 << 1,
}Encoder_Edge;

typedef struct Encoder {
  void (*UserData);
  GPIO_TypeDef* GPIOx_A;
  uint16_t GPIO_Pin_A;
  GPIO_TypeDef* GPIOx_B;
  uint16_t GPIO_Pin_B;
#if ENCODER_DE_JITTERING_MS > 0
  uint32_t LastLevelChangedMs;
  uint8_t LastChangedLevelA;
  uint8_t LastChangedLevelB;
#endif
  uint8_t LastStateA;
  uint8_t LastStateB;
  void (*OnTicked)(struct Encoder* sender, int8_t direction, Encoder_Edge edge);
} Encoder;


#define ENCODER_ON  GPIO_PIN_SET

void Encoder_Init(Encoder* encoder);
void Encoder_Scan(Encoder* encoder);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
