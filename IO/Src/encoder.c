#include "encoder.h"

void Encoder_Init(Encoder* encoder) {
  uint8_t levelA = HAL_GPIO_ReadPin(encoder->GPIOx_A, encoder->GPIO_Pin_A);
  uint8_t levelB = HAL_GPIO_ReadPin(encoder->GPIOx_B, encoder->GPIO_Pin_B);
  encoder->LastStateA = levelA;
  encoder->LastStateB = levelB;

#if ENCODER_DE_JITTERING_MS > 0
  uint32_t tickMs = HAL_GetTick();

  encoder->LastLevelChangedMs = tickMs;
  encoder->LastChangedLevelA = levelA;
  encoder->LastChangedLevelB = levelB;
#endif
}

void Encoder_Scan(Encoder* encoder) {
  uint8_t levelA = HAL_GPIO_ReadPin(encoder->GPIOx_A, encoder->GPIO_Pin_A);
  uint8_t levelB = HAL_GPIO_ReadPin(encoder->GPIOx_B, encoder->GPIO_Pin_B);

#if ENCODER_DE_JITTERING_MS > 0
  uint32_t tickMs = HAL_GetTick();

  // Level update
  if(encoder->LastChangedLevelA != levelA) {
    encoder->LastChangedLevelA = levelA;
    encoder->LastLevelChangedMs = tickMs;
  }
  if(encoder->LastChangedLevelB != levelB) {
    encoder->LastChangedLevelB = levelB;
    encoder->LastLevelChangedMs = tickMs;
  }

  // State update
  uint8_t stateA = encoder->LastStateA;
  uint8_t stateB = encoder->LastStateB;
  if(encoder->LastStateA != levelA && tickMs - encoder->LastLevelChangedMs > ENCODER_DE_JITTERING_MS) {
    stateA = levelA;
  }
  if(encoder->LastStateB != levelB && tickMs - encoder->LastLevelChangedMs > ENCODER_DE_JITTERING_MS) {
    stateB = levelB;
  }
#else
  uint8_t stateA = levelA;
  uint8_t stateB = levelB;
#endif

  // Tick check
  int8_t dir = 0, edge = 0;
  if (stateA != encoder->LastStateA) {
    edge = ENCODER_EDGE_A;
    if (stateA == ENCODER_ON) {
      if (stateB == ENCODER_ON) {
        dir--;
      }
      else {
        dir++;
      }
    }
    else {
      if (stateB == ENCODER_ON) {
        dir++;
      }
      else {
        dir--;
      }
    }
  }
  else if (stateB != encoder->LastStateB) {
    edge = ENCODER_EDGE_B;
    if (stateB == ENCODER_ON) {
      if (stateA == ENCODER_ON) {
        dir++;
      }
      else {
        dir--;
      }
    }
    else{
      if (stateA == ENCODER_ON) {
        dir--;
      }
      else {
        dir++;
      }
    }
  }
  
  // Struct update
  encoder->LastStateA = stateA;
  encoder->LastStateB = stateB;
  
  // callback
  if (dir != 0) {
    (*encoder->OnTicked)(encoder, dir, edge);
  }
}