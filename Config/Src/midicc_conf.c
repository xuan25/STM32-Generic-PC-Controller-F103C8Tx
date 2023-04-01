#include "midicc_conf.h"
#include "lighting_conf.h"

MIDICCBuffer* midiBuffers = NULL;

MIDICCBuffer* MIDI_GetBuffer(uint8_t channelNumber, uint8_t controllerNumber) {

  uint32_t midiBufferID = channelNumber | controllerNumber << 8;
  MIDICCBuffer* midiBuf = midiBuffers;
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

  MIDICCBuffer* midiBuf = MIDI_GetBuffer(channelNumber, controllerNumber);

  midiBuf->Value = value;

  Lighting_OnMIDICCChanged(channelNumber, controllerNumber, value);
}

uint8_t MIDICC_OnChangeDelta(uint8_t channelNumber, uint8_t controllerNumber, int8_t delta) {

  MIDICCBuffer* midiBuf = MIDI_GetBuffer(channelNumber, controllerNumber);

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