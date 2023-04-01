#include "inputs_conf.h"

#include "lighting_conf.h"
#include "usbd_hid_if.h"
#include "usbd_midi_if.h"
#include "key_matrix.h"
#include "pushable_dial.h"
#include "delay.h"
#include "midicc_conf.h"

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
  // MIDI: change delta (0 for toggle)
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
  ActionConfig PushKey;
  ActionConfig Clicked;
} DialConfig;

uint16_t ctrlState = 0x0000;
uint8_t radialButtonState = 0x00;
uint8_t keyboardModifierState = 0x00;
uint8_t keyboardKeyState[6] = {
  0, 0, 0, 0, 0, 0
};

ActionConfig matrixKeyConfigs[] = {
  { REPORT_NONE },
  { 
    .Type = REPORT_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 80,
    .Byte03 = 0,
  },
  { 
    .Type = REPORT_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 81,
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
};

DialConfig dialConfigs[] = {
  {
    .ReleasedCW = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_NEXT & 0xff,
      .Byte01 = CTRL_NEXT >> 8 & 0xff,
    },
    .ReleasedCCW = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_PREVIOUS & 0xff,
      .Byte01 = CTRL_PREVIOUS >> 8 & 0xff,
    },
    .PressedCW = {
      .Type = REPORT_MIDI,
      .Byte00 = 0,
      .Byte01 = 0,
      .Byte02 = 7,
      .Byte03 = 1,
    },
    .PressedCCW = {
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
    .Clicked = {
      .Type = REPORT_CONSUMER,
      .Byte00 = CTRL_PLAY_PAUSE & 0xff,
      .Byte01 = CTRL_PLAY_PAUSE >> 8 & 0xff,
    }
  },
  {
    .ReleasedCW = {
      .Type = REPORT_RADIAL,
      .Byte01 = ((int16_t)100) & 0xff,
      .Byte02 = ((int16_t)100) >> 8 & 0xff,
    },
    .ReleasedCCW = {
      .Type = REPORT_RADIAL,
      .Byte01 = ((int16_t)-100) & 0xff,
      .Byte02 = ((int16_t)-100) >> 8 & 0xff,
    },
    .PressedCW = {
      .Type = REPORT_RADIAL,
      .Byte01 = ((int16_t)20) & 0xff,
      .Byte02 = ((int16_t)20) >> 8 & 0xff,
    },
    .PressedCCW = {
      .Type = REPORT_RADIAL,
      .Byte01 = ((int16_t)-20) & 0xff,
      .Byte02 = ((int16_t)-20) >> 8 & 0xff,
    },
    .PushKey = {
      .Type = REPORT_RADIAL,
      .Byte00 = 1,
    },
  }
};

uint8_t Inputs_KeyboardStateAddKey(uint8_t key) {
  if (key == 0) {
    return 0;
  }
  for (int i = 0; i < sizeof(keyboardKeyState); i++) {
    if(keyboardKeyState[i] == 0) {
      keyboardKeyState[i] = key;
      return 1;
    }
  }
  return 0;
}

void Inputs_KeyboardStateRemoveKey(uint8_t key) {
  if (key == 0) {
    return 0;
  }
  for (int i = 0; i < sizeof(keyboardKeyState); i++) {
    if(keyboardKeyState[i] == key) {
      keyboardKeyState[i] = 0;
      return 1;
    }
  }
  return 0;
}

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
    keyboardModifierState = keyboardModifierState | actionConfig->Byte00;
    
    Inputs_KeyboardStateAddKey(actionConfig->Byte02);
    Inputs_KeyboardStateAddKey(actionConfig->Byte03);
    Inputs_KeyboardStateAddKey(actionConfig->Byte04);
    Inputs_KeyboardStateAddKey(actionConfig->Byte05);
    Inputs_KeyboardStateAddKey(actionConfig->Byte06);
    Inputs_KeyboardStateAddKey(actionConfig->Byte07);

    while(USBD_HID_SendKeyboardReport_FS(keyboardModifierState, actionConfig->Byte01, keyboardKeyState[0], keyboardKeyState[1], keyboardKeyState[2], keyboardKeyState[3], keyboardKeyState[4], keyboardKeyState[5]) != USBD_OK);
    break;
  case REPORT_RADIAL:
    if(actionConfig->Byte00) {
      radialButtonState = 1;
    }
    int16_t r = actionConfig->Byte01 | actionConfig->Byte02 << 8;
    while(USBD_HID_SendRadialReport_FS(radialButtonState, r, actionConfig->Byte03 | actionConfig->Byte04 << 8, actionConfig->Byte05 | actionConfig->Byte08 << 8, actionConfig->Byte07 | actionConfig->Byte08 << 8) != USBD_OK);
    break;
  case REPORT_MIDI:
    {
      uint8_t val = MIDICC_OnChangeDelta(actionConfig->Byte01, actionConfig->Byte02, (int8_t)actionConfig->Byte03);
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
    keyboardModifierState = keyboardModifierState & ~actionConfig->Byte00;

    Inputs_KeyboardStateRemoveKey(actionConfig->Byte02);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte03);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte04);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte05);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte06);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte07);

    while(USBD_HID_SendKeyboardReport_FS(keyboardModifierState, actionConfig->Byte01, keyboardKeyState[0], keyboardKeyState[1], keyboardKeyState[2], keyboardKeyState[3], keyboardKeyState[4], keyboardKeyState[5]) != USBD_OK);
    break;
  case REPORT_RADIAL:
    if(actionConfig->Byte00) {
      radialButtonState = 0;
      while(USBD_HID_SendRadialReport_FS(radialButtonState, 0, 0, 0, 0) != USBD_OK);
    }
    break;
  case REPORT_MIDI:
    break;
  default:
    break;
  }
}

uint8_t Inputs_OnKeyMatrixStateChanged(KeyMatrix* sender, MatrixKey* matrixKey, BinaryPushKeyState state) {
  int8_t keyID = matrixKey->X + matrixKey->Y * sender->Internal.Stride;

  Lighting_OnKeyMatrixStateChanged(keyID, state);

  ActionConfig* actionConfig = &matrixKeyConfigs[keyID];
  if(state == PushKeyPressed) {
    Inputs_ActionSet(actionConfig);
  } 
  else {
    Inputs_ActionReset(actionConfig);
  }
}

uint8_t Inputs_OnDial0ReleasedTicked(PushableDial* sender, int8_t direction) {
  Lighting_OnDialReleasedTicked(0, direction);

  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[0].ReleasedCW);
    Inputs_ActionReset(&dialConfigs[0].ReleasedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[0].ReleasedCCW);
    Inputs_ActionReset(&dialConfigs[0].ReleasedCCW);
  }

  return 1;
}

uint8_t Inputs_OnDial0PressedTicked(PushableDial* sender, int8_t direction) {
  Lighting_OnDialPressedTicked(0, direction);

  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[0].PressedCW);
    Inputs_ActionReset(&dialConfigs[0].PressedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[0].PressedCCW);
    Inputs_ActionReset(&dialConfigs[0].PressedCCW);
  }

  return 1;
}

uint8_t Inputs_OnDial0KeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked) {
  Lighting_OnDialKeyStateChanged(0, state, isDialTicked);

  if(state == PushKeyReleased) {
    Inputs_ActionReset(&dialConfigs[0].PushKey);
  } else {
    Inputs_ActionSet(&dialConfigs[0].PushKey);
  }

  if (state == PushKeyReleased && !isDialTicked) {
    Inputs_ActionSet(&dialConfigs[0].Clicked);
    Inputs_ActionReset(&dialConfigs[0].Clicked);
  }
  
  return 1;
}

uint8_t Inputs_OnDial1ReleasedTicked(PushableDial* sender, int8_t direction) {
  Lighting_OnDialReleasedTicked(1, direction);

  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[1].ReleasedCW);
    Inputs_ActionReset(&dialConfigs[1].ReleasedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[1].ReleasedCCW);
    Inputs_ActionReset(&dialConfigs[1].ReleasedCCW);
  }

  return 1;
}

uint8_t Inputs_OnDial1PressedTicked(PushableDial* sender, int8_t direction) {
  Lighting_OnDialPressedTicked(1, direction);

  if (direction > 0) {
    Inputs_ActionSet(&dialConfigs[1].PressedCW);
    Inputs_ActionReset(&dialConfigs[1].PressedCW);
  } else {
    Inputs_ActionSet(&dialConfigs[1].PressedCCW);
    Inputs_ActionReset(&dialConfigs[1].PressedCCW);
  }

  return 1;
}

uint8_t Inputs_OnDial1KeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked) {
  Lighting_OnDialKeyStateChanged(1, state, isDialTicked);

  if(state == PushKeyReleased) {
    Inputs_ActionReset(&dialConfigs[1].PushKey);
  } else {
    Inputs_ActionSet(&dialConfigs[1].PushKey);
  }

  if (state == PushKeyReleased && !isDialTicked) {
    Inputs_ActionSet(&dialConfigs[1].Clicked);
    Inputs_ActionReset(&dialConfigs[1].Clicked);
  }
  
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
  Delay_Us(GPIO_GENERIC_DELAY_US);
  PushableDial_Scan(pushableDial1_def);
  HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_RESET);

  PushableDial_Scan(pushableDial2_def);
}