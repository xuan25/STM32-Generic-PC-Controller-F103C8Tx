#include "inputs_conf.h"

#include "lighting_conf.h"
#include "usbd_hid_if.h"
#include "usbd_midi_if.h"
#include "key_matrix.h"
#include "pushable_dial.h"

typedef enum ReportType {
  REPORT_NONE,
  REPORT_CONSUMER,
  REPORT_KEYBOARD,
  REPORT_RADIAL,
  REPORT_MIDI,
} ReportType;

typedef struct ActionConfig {
  ReportType Type;
  // REPORT_CONSUMER: lower byte
  // REPORT_KEYBOARD: modifier
  // REPORT_RADIAL: Button
  // MIDI: cable number
  uint8_t Byte00;
  // REPORT_CONSUMER: higher byte
  // REPORT_KEYBOARD: OEM
  // REPORT_RADIAL: dial lower byte
  // MIDI: channel number
  uint8_t Byte01;
  // REPORT_KEYBOARD: key1
  // REPORT_RADIAL: dial higher byte
  // MIDI: controller number
  uint8_t Byte02;
  // REPORT_KEYBOARD: key2
  // REPORT_RADIAL: x lower byte
  // MIDI: step (0 for toggle)
  uint8_t Byte03;
  // REPORT_KEYBOARD: key3
  // REPORT_RADIAL: x higher byte
  uint8_t Byte04;
  // REPORT_KEYBOARD: key4
  // REPORT_RADIAL: y lower byte
  uint8_t Byte05;
  // REPORT_KEYBOARD: key5
  // REPORT_RADIAL: y higher byte
  uint8_t Byte06;
  // REPORT_KEYBOARD: key6
  // REPORT_RADIAL: width lower byte
  uint8_t Byte07;
  // REPORT_RADIAL: width higher byte
  uint8_t Byte08;
} ActionConfig;

typedef struct DialConfig {
  ActionConfig ReleasedCW;
  ActionConfig ReleasedCCW;
  ActionConfig PressedCW;
  ActionConfig PressedCCW;
  ActionConfig Clicked;
} DialConfig;

typedef struct MIDIBuffer {
  uint32_t ID;
  uint8_t Value;
  struct MIDIBuffer* Next;
} MIDIBuffer;

uint16_t ctrlState = 0x0000;

MIDIBuffer* midiBuffers = NULL;

ActionConfig matrixKeyConfigs[] = {
  { REPORT_NONE },
  { 
    .Type = REPORT_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 8,
    .Byte03 = 0,
  },
  { REPORT_NONE },
  { REPORT_NONE },

  { REPORT_NONE },
  { REPORT_NONE },
  { REPORT_NONE },
  { REPORT_NONE },

  { REPORT_NONE },
  { REPORT_NONE },
  { REPORT_NONE },
  { REPORT_NONE },

  { REPORT_NONE },
  { REPORT_NONE },
  { REPORT_NONE },
  { REPORT_NONE },
};

DialConfig dialConfigs[] = {
  {
    .ReleasedCW = {
      .Type = REPORT_MIDI,
      .Byte00 = 0,
      .Byte01 = 0,
      .Byte02 = 7,
      .Byte03 = 1,
    },
    .ReleasedCCW = {
      .Type = REPORT_MIDI,
      .Byte00 = 0,
      .Byte01 = 0,
      .Byte02 = 7,
      .Byte03 = -1,
    },
    // .PressedCW = {
    //   .Type = REPORT_CONSUMER,
    //   .Byte00 = CTRL_VOLUME_INCREMENT & 0xff,
    //   .Byte01 = CTRL_VOLUME_INCREMENT >> 8 & 0xff,
    // },
    // .PressedCCW = {
    //   .Type = REPORT_CONSUMER,
    //   .Byte00 = CTRL_VOLUME_DECREMENT & 0xff,
    //   .Byte01 = CTRL_VOLUME_DECREMENT >> 8 & 0xff,
    // },
    .PressedCW = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_VOLUME_INCREMENT & 0xff,
      .Byte01 = CTRL_VOLUME_INCREMENT >> 8 & 0xff,
    },
    .PressedCCW = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_VOLUME_DECREMENT & 0xff,
      .Byte01 = CTRL_VOLUME_DECREMENT >> 8 & 0xff,
    },
    .Clicked = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_PLAY_PAUSE & 0xff,
      .Byte01 = CTRL_PLAY_PAUSE >> 8 & 0xff,
    }
    // .Clicked = {
    //   .Type = REPORT_NONE
    // }
  },
  {
    .ReleasedCW = {
      .Type = REPORT_MIDI,
      .Byte00 = 0,
      .Byte01 = 0,
      .Byte02 = 7,
      .Byte03 = 1,
    },
    .ReleasedCCW = {
      .Type = REPORT_MIDI,
      .Byte00 = 0,
      .Byte01 = 0,
      .Byte02 = 7,
      .Byte03 = -1,
    },
    // .PressedCW = {
    //   .Type = REPORT_CONSUMER,
    //   .Byte00 = CTRL_VOLUME_INCREMENT & 0xff,
    //   .Byte01 = CTRL_VOLUME_INCREMENT >> 8 & 0xff,
    // },
    // .PressedCCW = {
    //   .Type = REPORT_CONSUMER,
    //   .Byte00 = CTRL_VOLUME_DECREMENT & 0xff,
    //   .Byte01 = CTRL_VOLUME_DECREMENT >> 8 & 0xff,
    // },
    .PressedCW = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_VOLUME_INCREMENT & 0xff,
      .Byte01 = CTRL_VOLUME_INCREMENT >> 8 & 0xff,
    },
    .PressedCCW = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_VOLUME_DECREMENT & 0xff,
      .Byte01 = CTRL_VOLUME_DECREMENT >> 8 & 0xff,
    },
    .Clicked = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_PLAY_PAUSE & 0xff,
      .Byte01 = CTRL_PLAY_PAUSE >> 8 & 0xff,
    }
    // .Clicked = {
    //   .Type = REPORT_NONE
    // }
  }
};

DialConfig dial2Config = {
  .ReleasedCW = {
    .Type = REPORT_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 7,
    .Byte03 = 1,
  },
  .ReleasedCCW = {
    .Type = REPORT_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 7,
    .Byte03 = -1,
  },
  // .PressedCW = {
  //   .Type = REPORT_CONSUMER,
  //   .Byte00 = CTRL_VOLUME_INCREMENT & 0xff,
  //   .Byte01 = CTRL_VOLUME_INCREMENT >> 8 & 0xff,
  // },
  // .PressedCCW = {
  //   .Type = REPORT_CONSUMER,
  //   .Byte00 = CTRL_VOLUME_DECREMENT & 0xff,
  //   .Byte01 = CTRL_VOLUME_DECREMENT >> 8 & 0xff,
  // },
  .PressedCW = {
    .Type = REPORT_CONSUMER,
    .Byte00 = CTRL_VOLUME_INCREMENT & 0xff,
    .Byte01 = CTRL_VOLUME_INCREMENT >> 8 & 0xff,
  },
  .PressedCCW = {
    .Type = REPORT_CONSUMER,
    .Byte00 = CTRL_VOLUME_DECREMENT & 0xff,
    .Byte01 = CTRL_VOLUME_DECREMENT >> 8 & 0xff,
  },
  .Clicked = {
    .Type = REPORT_CONSUMER,
    .Byte00 = CTRL_PLAY_PAUSE & 0xff,
    .Byte01 = CTRL_PLAY_PAUSE >> 8 & 0xff,
  }
  // .Clicked = {
  //   .Type = REPORT_NONE
  // }
};

void Inputs_ActionSet(ActionConfig* actionConfig) {
  switch (actionConfig->Type)
  {
  case REPORT_NONE:
    break;
  case REPORT_CONSUMER: 
    {
      uint16_t flag = actionConfig->Byte00 | actionConfig->Byte01 << 8;
      ctrlState = ctrlState | flag;
      while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    }
    break;
  case REPORT_KEYBOARD:
    while(USBD_HID_SendKeyboardReport_FS(actionConfig->Byte00, actionConfig->Byte01, actionConfig->Byte02, actionConfig->Byte03, actionConfig->Byte05, actionConfig->Byte05, actionConfig->Byte06, actionConfig->Byte07) != USBD_OK);
    break;
  case REPORT_RADIAL:
    while(USBD_HID_SendRadialReport_FS(actionConfig->Byte00, actionConfig->Byte01 | actionConfig->Byte02 << 8, actionConfig->Byte03 | actionConfig->Byte04 << 8, actionConfig->Byte05 | actionConfig->Byte08 << 8, actionConfig->Byte07 | actionConfig->Byte08 << 8) != USBD_OK);
    break;
  case REPORT_MIDI:
    {
      uint32_t midiBufferID = actionConfig->Byte00 | actionConfig->Byte01 << 8 | actionConfig->Byte02 << 16;
      MIDIBuffer* midiBuf = midiBuffers;
      if(midiBuffers == NULL) {
        midiBuffers = calloc(1, sizeof(MIDIBuffer));
        midiBuf = midiBuffers;
        midiBuf->ID = midiBufferID;
      } else {
        while (midiBuf->ID != midiBufferID && midiBuf->Next != NULL) {
          midiBuf = midiBuf->Next;
        }
        if(midiBuf->ID != midiBufferID) {
          midiBuf->Next = calloc(1, sizeof(MIDIBuffer));
          midiBuf = midiBuf->Next;
          midiBuf->ID = midiBufferID;
        }
      }

      int16_t val = midiBuf->Value;
      int8_t step = (int8_t)actionConfig->Byte03;
      if(step != 0) {
        val += step;
      } else {
        val = 127 - val;
      }
      if (val >= 128) {
        val = 127;
      }
      if (val < 0) {
        val = 0;
      }
      midiBuf->Value = val;
      while(USBD_MIDI_SendCCMessage_FS(actionConfig->Byte00, actionConfig->Byte01, actionConfig->Byte02, val) != USBD_OK);
    }
    break;
  default:
    break;
  }
}

void Inputs_ActionReset(ActionConfig* actionConfig) {
  switch (actionConfig->Type)
  {
  case REPORT_NONE:
    break;
  case REPORT_CONSUMER: 
    {
      uint16_t flag = actionConfig->Byte00 | actionConfig->Byte01 << 8;
      ctrlState = ctrlState & ~flag;
      while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    }
    break;
  case REPORT_KEYBOARD:
    // TODO: handle key released
    while(USBD_HID_SendKeyboardReport_FS(0, 0, 0, 0, 0, 0, 0, 0) != USBD_OK);
    break;
  case REPORT_RADIAL:
    // TODO: handle key released
    while(USBD_HID_SendRadialReport_FS(0, 0, 0, 0, 0) != USBD_OK);
    break;
  case REPORT_MIDI:
    break;
  default:
    break;
  }
}

uint8_t Inputs_OnKeyMatrixStateChanged(KeyMatrix* sender, MatrixKey* matrixKey, BinaryPushKeyState state) {
  int8_t keyID = matrixKey->X + matrixKey->Y * sender->Internal.Stride;
  ActionConfig* actionConfig = &matrixKeyConfigs[keyID];
  if(state == PushKeyPressed) {
    Inputs_ActionSet(actionConfig);
  } 
  else {
    Inputs_ActionReset(actionConfig);
  }

  Lighting_OnKeyMatrixStateChanged(keyID, state);
}

uint8_t Inputs_OnDial0ReleasedTicked(PushableDial* sender, int8_t direction) {
  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[0].ReleasedCW);
    Inputs_ActionReset(&dialConfigs[0].ReleasedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[0].ReleasedCCW);
    Inputs_ActionReset(&dialConfigs[0].ReleasedCCW);
  }

  Lighting_OnDialReleasedTicked(0, direction);

  return 1;
}

uint8_t Inputs_OnDial0PressedTicked(PushableDial* sender, int8_t direction) {
  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[0].PressedCW);
    Inputs_ActionReset(&dialConfigs[0].PressedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[0].PressedCCW);
    Inputs_ActionReset(&dialConfigs[0].PressedCCW);
  }

  Lighting_OnDialPressedTicked(0, direction);

  return 1;
}

uint8_t Inputs_OnDial0KeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked) {
  if (state == PushKeyReleased && !isDialTicked) {
    Inputs_ActionSet(&dialConfigs[0].Clicked);
    Inputs_ActionReset(&dialConfigs[0].Clicked);
  }
  
  Lighting_OnDialKeyStateChanged(0, state, isDialTicked);

  return 1;
}

uint8_t Inputs_OnDial1ReleasedTicked(PushableDial* sender, int8_t direction) {
  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[1].ReleasedCW);
    Inputs_ActionReset(&dialConfigs[1].ReleasedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[1].ReleasedCCW);
    Inputs_ActionReset(&dialConfigs[1].ReleasedCCW);
  }

  Lighting_OnDialReleasedTicked(1, direction);

  return 1;
}

uint8_t Inputs_OnDial1PressedTicked(PushableDial* sender, int8_t direction) {
  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[1].PressedCW);
    Inputs_ActionReset(&dialConfigs[1].PressedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[1].PressedCCW);
    Inputs_ActionReset(&dialConfigs[1].PressedCCW);
  }

  Lighting_OnDialPressedTicked(1, direction);

  return 1;
}

uint8_t Inputs_OnDial1KeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked) {
  if (state == PushKeyReleased && !isDialTicked) {
    Inputs_ActionSet(&dialConfigs[1].Clicked);
    Inputs_ActionReset(&dialConfigs[1].Clicked);
  }
  
  Lighting_OnDialKeyStateChanged(1, state, isDialTicked);

  return 1;
}

KeyMatrix* keyMatrix_def = &((KeyMatrix){
  .MatrixKeys = ((MatrixKey*[]){
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 0,
      .Y = 0,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 0,
      .Y = 1,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 0,
      .Y = 2,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 0,
      .Y = 3,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 1,
      .Y = 0,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 1,
      .Y = 1,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 1,
      .Y = 2,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 1,
      .Y = 3,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 2,
      .Y = 0,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 2,
      .Y = 1,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 3,
      .Y = 0,
    }),
    &((MatrixKey){
      .Key = &((Key){
        
      }),
      .X = 3,
      .Y = 1,
    }),
    NULL
  }),
  .Rows = ((GPIO_Pin*[]){
    &((GPIO_Pin){
      .GPIOx = ROW_0_GPIO_Port,
      .GPIO_Pin = ROW_0_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = ROW_1_GPIO_Port,
      .GPIO_Pin = ROW_1_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = ROW_2_GPIO_Port,
      .GPIO_Pin = ROW_2_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = ROW_3_GPIO_Port,
      .GPIO_Pin = ROW_3_Pin,
    }),
    NULL
  }),
  .Cols = ((GPIO_Pin*[]){
    &((GPIO_Pin){
      .GPIOx = COL_0_GPIO_Port,
      .GPIO_Pin = COL_0_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = COL_1_GPIO_Port,
      .GPIO_Pin = COL_1_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = COL_2_GPIO_Port,
      .GPIO_Pin = COL_2_Pin,
    }),
    &((GPIO_Pin){
      .GPIOx = COL_3_GPIO_Port,
      .GPIO_Pin = COL_3_Pin,
    }),
    NULL
  }),
  .ReleasedLevel = GPIO_PIN_RESET,
  .OnStateChanged = Inputs_OnKeyMatrixStateChanged,
});

PushableDial* pushableDial1_def = &((PushableDial){
  .ReleasedDial = &((Dial){
    .Encoder = &((Encoder){
      .PinA = &((GPIO_Pin){
        .GPIOx = ENC_1_A_GPIO_Port,
        .GPIO_Pin = ENC_1_A_Pin,
      }),
      .PinB = &((GPIO_Pin){
        .GPIOx = ENC_1_B_GPIO_Port,
        .GPIO_Pin = ENC_1_B_Pin,
      }),
      .OffLevel = GPIO_PIN_RESET,
    }),
    .TickInterval = 5,
    .ResetDelayMs = 500,
  }),
  .PressedDial = &((Dial){
    .Encoder = &((Encoder){
      .PinA = &((GPIO_Pin){
        .GPIOx = ENC_1_A_GPIO_Port,
        .GPIO_Pin = ENC_1_A_Pin,
      }),
      .PinB = &((GPIO_Pin){
        .GPIOx = ENC_1_B_GPIO_Port,
        .GPIO_Pin = ENC_1_B_Pin,
      }),
      .OffLevel = GPIO_PIN_RESET,
    }),
    .TickInterval = 2,
    .ResetDelayMs = 500,
  }),
  .PushKey = &((BinaryPushKey){
    .Key = &((Key){
      
    }),
    .Pin = &((GPIO_Pin){
      .GPIOx = COL_0_GPIO_Port,
      .GPIO_Pin = COL_0_Pin,
    }),
    .ReleasedLevel = GPIO_PIN_RESET
  }),
  .OnReleasedDialTicked = Inputs_OnDial0ReleasedTicked,
  .OnPressedDialTicked = Inputs_OnDial0PressedTicked,
  .OnPushKeyStateChanged = Inputs_OnDial0KeyStateChanged,
});

PushableDial* pushableDial2_def = &((PushableDial){
  .ReleasedDial = &((Dial){
    .Encoder = &((Encoder){
      .PinA = &((GPIO_Pin){
        .GPIOx = ENC_2_A_GPIO_Port,
        .GPIO_Pin = ENC_2_A_Pin,
      }),
      .PinB = &((GPIO_Pin){
        .GPIOx = ENC_2_B_GPIO_Port,
        .GPIO_Pin = ENC_2_B_Pin,
      }),
      .OffLevel = GPIO_PIN_RESET,
    }),
    .TickInterval = 5,
    .ResetDelayMs = 500,
  }),
  .PressedDial = &((Dial){
    .Encoder = &((Encoder){
      .PinA = &((GPIO_Pin){
        .GPIOx = ENC_2_A_GPIO_Port,
        .GPIO_Pin = ENC_2_A_Pin,
      }),
      .PinB = &((GPIO_Pin){
        .GPIOx = ENC_2_B_GPIO_Port,
        .GPIO_Pin = ENC_2_B_Pin,
      }),
      .OffLevel = GPIO_PIN_RESET,
    }),
    .TickInterval = 2,
    .ResetDelayMs = 500,
  }),
  .PushKey = &((BinaryPushKey){
    .Key = &((Key){
      
    }),
    .Pin = &((GPIO_Pin){
      .GPIOx = ENC_2_P_GPIO_Port,
      .GPIO_Pin = ENC_2_P_Pin,
    }),
    .ReleasedLevel = GPIO_PIN_RESET
  }),
  .OnReleasedDialTicked = Inputs_OnDial1ReleasedTicked,
  .OnPressedDialTicked = Inputs_OnDial1PressedTicked,
  .OnPushKeyStateChanged = Inputs_OnDial1KeyStateChanged,
});

void Inputs_Init() {
  KeyMatrix_Init(keyMatrix_def);
  PushableDial_Init(pushableDial1_def);
  PushableDial_Init(pushableDial2_def);
}

void Inputs_Scan() {
  KeyMatrix_Scan(keyMatrix_def);

  HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  PushableDial_Scan(pushableDial1_def);
  HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_RESET);

  PushableDial_Scan(pushableDial2_def);
}