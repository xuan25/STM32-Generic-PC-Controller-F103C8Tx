/**
  ******************************************************************************
  * @file    midicc_conf.h
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Xuan25.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __MIDICC_CONF_H
#define __MIDICC_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

typedef struct MIDICCBuffer {
  uint32_t ID;
  uint8_t Value;
  struct MIDICCBuffer *Next;
} MIDICCBuffer;

extern MIDICCBuffer *midiBuffers;

void MIDICC_OnChanged(uint8_t channelNumber, uint8_t controllerNumber, uint8_t value);
uint8_t MIDICC_OnChangeDelta(uint8_t channelNumber, uint8_t controllerNumber, int8_t delta);

#ifdef __cplusplus
}
#endif

#endif /* __MIDICC_CONF_H */
