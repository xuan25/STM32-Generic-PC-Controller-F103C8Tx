/**
  ******************************************************************************
  * @file    midicc_conf.c
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

#include "midicc_conf.h"
#include "lighting_conf.h"

MIDICCBuffer *midiBuffers = NULL;

MIDICCBuffer *MIDI_GetBuffer(uint8_t channelNumber, uint8_t controllerNumber) {

  uint32_t midiBufferID = channelNumber | controllerNumber << 8;
  MIDICCBuffer *midiBuf = midiBuffers;
  if(midiBuffers == NULL) {
    // init buffer list
    midiBuffers = calloc(1, sizeof(MIDICCBuffer));
    midiBuf = midiBuffers;
    midiBuf->ID = midiBufferID;
  } else {
    // find buffer
    while (midiBuf->ID != midiBufferID && midiBuf->Next != NULL) {
      midiBuf = midiBuf->Next;
    }
    if(midiBuf->ID != midiBufferID) {
      // create new
      midiBuf->Next = calloc(1, sizeof(MIDICCBuffer));
      midiBuf = midiBuf->Next;
      midiBuf->ID = midiBufferID;
    }
  }

  return midiBuf;
}

void MIDICC_OnChanged(uint8_t channelNumber, uint8_t controllerNumber, uint8_t value) {

  MIDICCBuffer *midiBuf = MIDI_GetBuffer(channelNumber, controllerNumber);

  midiBuf->Value = value;

  Lighting_OnMIDICCChanged(channelNumber, controllerNumber, value);
}

void USBD_MIDI_DataOutHandler(uint8_t *usbRxBuffer, uint8_t usbRxBufferLength)
{
  uint8_t code_index_number = usbRxBuffer[0] & 0xf;
  uint8_t cable_number = usbRxBuffer[0] >> 4 & 0xf;
  uint8_t channel_number = usbRxBuffer[1] & 0xf;
  uint8_t message_number = usbRxBuffer[1] >> 4 & 0xf;
  uint8_t controller_number = usbRxBuffer[2];
  uint8_t new_value = usbRxBuffer[3];
  
  MIDICC_OnChanged(channel_number, controller_number, new_value);
}

uint8_t MIDICC_OnChangeDelta(uint8_t channelNumber, uint8_t controllerNumber, int8_t delta) {

  MIDICCBuffer *midiBuf = MIDI_GetBuffer(channelNumber, controllerNumber);

  int16_t value = midiBuf->Value;
  if(delta != 0) {
    value += delta;
  } else {
    value = 127 - value;
  }
  if (value >= 128) {
    value = 127;
  }
  if (value < 0) {
    value = 0;
  }
  midiBuf->Value = value;

  Lighting_OnMIDICCChanged(channelNumber, controllerNumber, value);

  return value;
}