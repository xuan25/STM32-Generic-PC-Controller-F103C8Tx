#include "inputs_conf.h"

#include "lighting_conf.h"
#include "usbd_hid_if.h"
#include "usbd_midi_if.h"
#include "key_matrix.h"
#include "pushable_dial.h"
#include "delay.h"
#include "midicc_conf.h"


uint8_t Inputs_KeyboardStateAddKey(uint8_t key);
uint8_t Inputs_KeyboardStateRemoveKey(uint8_t key);
void Inputs_ActionSet(ActionConfig* actionConfig);
void Inputs_ActionReset(ActionConfig* actionConfig);
uint8_t Inputs_OnKeyMatrixStateChanged(KeyMatrix* sender, MatrixKey* matrixKey, BinaryPushKeyState state);
uint8_t Inputs_OnPushableDialReleasedTicked(PushableDial* sender, int8_t direction);
uint8_t Inputs_OnPushableDialPressedTicked(PushableDial* sender, int8_t direction);
uint8_t Inputs_OnPushableDialKeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked);

uint16_t ctrlState = 0x0000;
uint8_t keyboardModifierState = 0x00;
uint8_t mouseButtonsState = 0x00;
uint8_t radialButtonState = 0x00;


uint8_t keyboardKeyState[6] = {
  0, 0, 0, 0, 0, 0
};

uint8_t matrixKeyConfigsMapping[] = {
  0, 1, 2, 3, 
  4, 5, 6, 7, 
  8, 9, 0, 0,
  10, 11, 0, 0
};

ActionConfig actionConfigs[] = {
  // Dial ReleasedCW
  {
    .Type = ACTION_CTRL,
    .Byte00 = CTRL_NEXT & 0xff,
    .Byte01 = CTRL_NEXT >> 8 & 0xff,
  },
  {
    .Type = ACTION_RADIAL,
    .Byte01 = ((int16_t)100) & 0xff,
    .Byte02 = ((int16_t)100) >> 8 & 0xff,
  },
  // Dial ReleasedCCW
  {
    .Type = ACTION_CTRL,
    .Byte00 = CTRL_PREVIOUS & 0xff,
    .Byte01 = CTRL_PREVIOUS >> 8 & 0xff,
  },
  {
    .Type = ACTION_RADIAL,
    .Byte01 = ((int16_t)-100) & 0xff,
    .Byte02 = ((int16_t)-100) >> 8 & 0xff,
  },
  // Dial PressedCW
  {
    .Type = ACTION_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 7,
    .Byte03 = 1,
  },
  {
    .Type = ACTION_RADIAL,
    .Byte01 = ((int16_t)20) & 0xff,
    .Byte02 = ((int16_t)20) >> 8 & 0xff,
  },
  // Dial PressedCCW
  {
    .Type = ACTION_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 7,
    .Byte03 = -1,
  },
  {
    .Type = ACTION_RADIAL,
    .Byte01 = ((int16_t)-20) & 0xff,
    .Byte02 = ((int16_t)-20) >> 8 & 0xff,
  },
  // Dial PushKey
  {
    ACTION_NONE
  },
  {
    .Type = ACTION_RADIAL,
    .Byte00 = 1,
  },
  // Dial Clicked
  {
    .Type = ACTION_CTRL,
    .Byte00 = CTRL_PLAY_PAUSE & 0xff,
    .Byte01 = CTRL_PLAY_PAUSE >> 8 & 0xff,
  },
  {
    ACTION_NONE
  },
  // MatrixKeys
  // row 0
  { ACTION_NONE },
  { 
    .Type = ACTION_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 80,
    .Byte03 = 0,
  },
  { 
    .Type = ACTION_MIDI,
    .Byte00 = 0,
    .Byte01 = 0,
    .Byte02 = 81,
    .Byte03 = 0,
  },
  { ACTION_NONE },
  // row 1
  { ACTION_NONE },
  { ACTION_NONE },
  { ACTION_NONE },
  { ACTION_NONE },
  // row 2
  { ACTION_NONE },
  { ACTION_NONE },
  // row 3
  { ACTION_NONE },
  {
    .Type = ACTION_CTRL,
    .Byte00 = CTRL_PLAY_PAUSE & 0xff,
    .Byte01 = CTRL_PLAY_PAUSE >> 8 & 0xff,
  },
};

KeyMatrix* keyMatrix = &((KeyMatrix){
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

PushableDial* pushableDials[] = {
  &((PushableDial){
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
    .OnReleasedDialTicked = Inputs_OnPushableDialReleasedTicked,
    .OnPressedDialTicked = Inputs_OnPushableDialPressedTicked,
    .OnPushKeyStateChanged = Inputs_OnPushableDialKeyStateChanged,
  }),
  &((PushableDial){
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
    .OnReleasedDialTicked = Inputs_OnPushableDialReleasedTicked,
    .OnPressedDialTicked = Inputs_OnPushableDialPressedTicked,
    .OnPushKeyStateChanged = Inputs_OnPushableDialKeyStateChanged,
  })
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

uint8_t Inputs_KeyboardStateRemoveKey(uint8_t key) {
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
  case ACTION_NONE:
    break;
  case ACTION_CTRL: 
    {
      uint16_t flag = actionConfig->Byte00 | actionConfig->Byte01 << 8;
      ctrlState = ctrlState | flag;
      while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    }
    break;
  case ACTION_KEYBOARD:
    keyboardModifierState = keyboardModifierState | actionConfig->Byte00;
    
    Inputs_KeyboardStateAddKey(actionConfig->Byte02);
    Inputs_KeyboardStateAddKey(actionConfig->Byte03);
    Inputs_KeyboardStateAddKey(actionConfig->Byte04);
    Inputs_KeyboardStateAddKey(actionConfig->Byte05);
    Inputs_KeyboardStateAddKey(actionConfig->Byte06);
    Inputs_KeyboardStateAddKey(actionConfig->Byte07);

    while(USBD_HID_SendKeyboardReport_FS(keyboardModifierState, actionConfig->Byte01, keyboardKeyState[0], keyboardKeyState[1], keyboardKeyState[2], keyboardKeyState[3], keyboardKeyState[4], keyboardKeyState[5]) != USBD_OK);
    break;
  case ACTION_MOUSE:
    mouseButtonsState = mouseButtonsState | actionConfig->Byte00;

    while(USBD_HID_SendMouseReport_FS(mouseButtonsState, actionConfig->Byte01, actionConfig->Byte02, actionConfig->Byte03) != USBD_OK);
    break;
  case ACTION_RADIAL:
    if(actionConfig->Byte00) {
      radialButtonState = 1;
    }
    int16_t r = actionConfig->Byte01 | actionConfig->Byte02 << 8;
    while(USBD_HID_SendRadialReport_FS(radialButtonState, r, actionConfig->Byte03 | actionConfig->Byte04 << 8, actionConfig->Byte05 | actionConfig->Byte08 << 8, actionConfig->Byte07 | actionConfig->Byte08 << 8) != USBD_OK);
    break;
  case ACTION_MIDI:
    {
      if(actionConfig->Byte03 >> 7) {
        // abs value
        MIDICC_OnChanged(actionConfig->Byte01, actionConfig->Byte02, actionConfig->Byte03 & 0x7F);
        while(USBD_MIDI_SendCCMessage_FS(actionConfig->Byte00, actionConfig->Byte01, actionConfig->Byte02, actionConfig->Byte03 & 0x7F) != USBD_OK);
      } else {
        // rel value
        uint8_t val = MIDICC_OnChangeDelta(actionConfig->Byte01, actionConfig->Byte02, (int8_t)actionConfig->Byte03);
        while(USBD_MIDI_SendCCMessage_FS(actionConfig->Byte00, actionConfig->Byte01, actionConfig->Byte02, val) != USBD_OK);
      }
    }
    break;
  default:
    break;
  }
}

void Inputs_ActionReset(ActionConfig* actionConfig) {
  switch (actionConfig->Type)
  {
  case ACTION_NONE:
    break;
  case ACTION_CTRL: 
    {
      uint16_t flag = actionConfig->Byte00 | actionConfig->Byte01 << 8;
      ctrlState = ctrlState & ~flag;
      while(USBD_HID_SendCtrlReport_FS(ctrlState) != USBD_OK);
    }
    break;
  case ACTION_KEYBOARD:
    keyboardModifierState = keyboardModifierState & ~actionConfig->Byte00;

    Inputs_KeyboardStateRemoveKey(actionConfig->Byte02);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte03);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte04);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte05);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte06);
    Inputs_KeyboardStateRemoveKey(actionConfig->Byte07);

    while(USBD_HID_SendKeyboardReport_FS(keyboardModifierState, actionConfig->Byte01, keyboardKeyState[0], keyboardKeyState[1], keyboardKeyState[2], keyboardKeyState[3], keyboardKeyState[4], keyboardKeyState[5]) != USBD_OK);
    break;
  case ACTION_MOUSE:
    if(actionConfig->Byte00) {
      mouseButtonsState = mouseButtonsState & ~actionConfig->Byte00;
      while(USBD_HID_SendMouseReport_FS(mouseButtonsState, actionConfig->Byte01, actionConfig->Byte02, actionConfig->Byte03) != USBD_OK);
    }
    break;
  case ACTION_RADIAL:
    if(actionConfig->Byte00) {
      radialButtonState = 0;
      while(USBD_HID_SendRadialReport_FS(radialButtonState, 0, 0, 0, 0) != USBD_OK);
    }
    break;
  case ACTION_MIDI:
    // ignored
    break;
  default:
    break;
  }
}

uint8_t Inputs_OnKeyMatrixStateChanged(KeyMatrix* sender, MatrixKey* matrixKey, BinaryPushKeyState state) {
  int8_t keyID = matrixKey->X + matrixKey->Y * sender->Internal.Stride;

  Lighting_OnKeyMatrixStateChanged(keyID, state);

  ActionConfig* actionConfig = ACTION_MatrixKey_00 + matrixKeyConfigsMapping[keyID];
  if(state == PushKeyPressed) {
    Inputs_ActionSet(actionConfig);
  } 
  else {
    Inputs_ActionReset(actionConfig);
  }
}

uint8_t Inputs_OnPushableDialReleasedTicked(PushableDial* sender, int8_t direction) {
  int dialID;
  for (dialID = 0; dialID < sizeof(pushableDials); dialID++) {
    if(sender == pushableDials[dialID]) {
      break;
    }
  }

  Lighting_OnDialReleasedTicked(dialID, direction);

  if (direction > 0) {
    Inputs_ActionSet(ACTION_DIAL_0_RELEASED_CW + dialID);
    Inputs_ActionReset(ACTION_DIAL_0_RELEASED_CW + dialID);
  } else {
    Inputs_ActionSet(ACTION_DIAL_0_RELEASED_CCW + dialID);
    Inputs_ActionReset(ACTION_DIAL_0_RELEASED_CCW + dialID);
  }

  return 1;
}

uint8_t Inputs_OnPushableDialPressedTicked(PushableDial* sender, int8_t direction) {
  int dialID;
  for (dialID = 0; dialID < sizeof(pushableDials); dialID++) {
    if(sender == pushableDials[dialID]) {
      break;
    }
  }

  Lighting_OnDialPressedTicked(dialID, direction);

  if (direction > 0) {
    Inputs_ActionSet(ACTION_DIAL_0_PRESSED_CW + dialID);
    Inputs_ActionReset(ACTION_DIAL_0_PRESSED_CW + dialID);
  } else {
    Inputs_ActionSet(ACTION_DIAL_0_PRESSED_CCW + dialID);
    Inputs_ActionReset(ACTION_DIAL_0_PRESSED_CCW + dialID);
  }

  return 1;
}

uint8_t Inputs_OnPushableDialKeyStateChanged(PushableDial* sender, BinaryPushKeyState state, uint8_t isDialTicked) {
  int dialID;
  for (dialID = 0; dialID < sizeof(pushableDials); dialID++) {
    if(sender == pushableDials[dialID]) {
      break;
    }
  }
  
  Lighting_OnDialKeyStateChanged(dialID, state, isDialTicked);

  if(state == PushKeyReleased) {
    Inputs_ActionReset(ACTION_DIAL_0_PUSH_KEY + dialID);
  } else {
    Inputs_ActionSet(ACTION_DIAL_0_PUSH_KEY + dialID);
  }

  if (state == PushKeyReleased && !isDialTicked) {
    Inputs_ActionSet(ACTION_DIAL_0_CLICKED + dialID);
    Inputs_ActionReset(ACTION_DIAL_0_CLICKED + dialID);
  }
  
  return 1;
}

void Inputs_Init() {
  KeyMatrix_Init(keyMatrix);
  PushableDial_Init(pushableDials[0]);
  PushableDial_Init(pushableDials[1]);
}

void Inputs_Scan() {
  KeyMatrix_Scan(keyMatrix);

  HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_SET);
  Delay_Us(GPIO_GENERIC_DELAY_US);
  PushableDial_Scan(pushableDials[0]);
  HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, GPIO_PIN_RESET);

  PushableDial_Scan(pushableDials[1]);
}
