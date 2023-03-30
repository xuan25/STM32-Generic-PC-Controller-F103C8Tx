#include "encoder.h"

void Encoder_Init(Encoder* encoder) {
  uint8_t levelA = HAL_GPIO_ReadPin(encoder->PinA->GPIOx, encoder->PinA->GPIO_Pin);
  uint8_t levelB = HAL_GPIO_ReadPin(encoder->PinB->GPIOx, encoder->PinB->GPIO_Pin);
  encoder->Internal.LastStateA = levelA;
  encoder->Internal.LastStateB = levelB;

#if ENCODER_DEBOUNCE_MS > 0
  uint32_t tickMs = HAL_GetTick();

  encoder->Internal.LastLevelChangedMs = tickMs;
  encoder->Internal.LastChangedLevelA = levelA;
  encoder->Internal.LastChangedLevelB = levelB;
#endif
}

void Encoder_Scan(Encoder* encoder) {
  uint8_t levelA = HAL_GPIO_ReadPin(encoder->PinA->GPIOx, encoder->PinA->GPIO_Pin);
  uint8_t levelB = HAL_GPIO_ReadPin(encoder->PinB->GPIOx, encoder->PinB->GPIO_Pin);

#if ENCODER_DEBOUNCE_MS > 0
  uint32_t tickMs = HAL_GetTick();

  // Level update
  if(encoder->Internal.LastChangedLevelA != levelA) {
    encoder->Internal.LastChangedLevelA = levelA;
    encoder->Internal.LastLevelChangedMs = tickMs;
  }
  if(encoder->Internal.LastChangedLevelB != levelB) {
    encoder->Internal.LastChangedLevelB = levelB;
    encoder->Internal.LastLevelChangedMs = tickMs;
  }

  // State update
  uint8_t stateA = encoder->Internal.LastStateA;
  uint8_t stateB = encoder->Internal.LastStateB;
  if(encoder->Internal.LastStateA != levelA && tickMs - encoder->Internal.LastLevelChangedMs > ENCODER_DEBOUNCE_MS) {
    stateA = levelA;
  }
  if(encoder->Internal.LastStateB != levelB && tickMs - encoder->Internal.LastLevelChangedMs > ENCODER_DEBOUNCE_MS) {
    stateB = levelB;
  }
#else
  uint8_t stateA = levelA;
  uint8_t stateB = levelB;
#endif

  // Tick check
  int8_t dir = 0, edge = 0;
  if (stateA != encoder->Internal.LastStateA) {
    edge = ENCODER_EDGE_A;
    if (stateA != encoder->OffLevel) {
      if (stateB != encoder->OffLevel) {
        dir--;
      }
      else {
        dir++;
      }
    }
    else {
      if (stateB != encoder->OffLevel) {
        dir++;
      }
      else {
        dir--;
      }
    }
  }
  else if (stateB != encoder->Internal.LastStateB) {
    edge = ENCODER_EDGE_B;
    if (stateB != encoder->OffLevel) {
      if (stateA != encoder->OffLevel) {
        dir++;
      }
      else {
        dir--;
      }
    }
    else{
      if (stateA != encoder->OffLevel) {
        dir--;
      }
      else {
        dir++;
      }
    }
  }
  
  // Struct update
  encoder->Internal.LastStateA = stateA;
  encoder->Internal.LastStateB = stateB;
  
  // callback
  if (dir != 0) {
    if(encoder->OnTicked == NULL || !encoder->OnTicked(encoder, dir, edge)) {
      if(encoder->Internal.OnTicked != NULL) {
        encoder->Internal.OnTicked(encoder, dir, edge);
      }
    }
    
  }
}