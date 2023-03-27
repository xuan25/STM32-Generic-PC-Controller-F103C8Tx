#include "pushable_wheel.h"

void PushableWheel_OnReleasedWheelTicked(Wheel *sender, int8_t direction);
void PushableWheel_OnPressedWheelTicked(Wheel *sender, int8_t direction);
void PushableWheel_OnPushKeyStateChanged(GPIOKey *sender, BinaryKeyState state);

void PushableWheel_Init(PushableWheel* pushableWheel) {
  if(pushableWheel->ReleasedWheel != NULL) {
    pushableWheel->ReleasedWheel->UserData = pushableWheel;
    pushableWheel->ReleasedWheel->OnTicked = PushableWheel_OnReleasedWheelTicked;
    Wheel_Init(pushableWheel->ReleasedWheel);
  }
  
  if(pushableWheel->ReleasedWheel != NULL) {
    pushableWheel->PressedWheel->UserData = pushableWheel;
    pushableWheel->PressedWheel->OnTicked = PushableWheel_OnPressedWheelTicked;
    Wheel_Init(pushableWheel->PressedWheel);
  }

  pushableWheel->PushKey->UserData = pushableWheel;
  pushableWheel->PushKey->OnStateChanged = PushableWheel_OnPushKeyStateChanged;
  GPIOKey_Init(pushableWheel->PushKey);
}

void PushableWheel_Scan(PushableWheel* pushableWheel) {
  Wheel_Scan(pushableWheel->PressedWheel);
  Wheel_Scan(pushableWheel->ReleasedWheel);
  GPIOKey_Scan(pushableWheel->PushKey);
}

void PushableWheel_OnReleasedWheelTicked(Wheel *sender, int8_t direction) {
  PushableWheel* pushableWheel = (PushableWheel*)sender->UserData;
  if(pushableWheel->PushKey->Key->State == pushableWheel->PushKey->ReleasedLevel) {
    pushableWheel->LastUpdatedItem = ReleasedWheel;
    pushableWheel->OnReleasedWheelTicked(pushableWheel, direction);
  }
}

void PushableWheel_OnPressedWheelTicked(Wheel *sender, int8_t direction){
  PushableWheel* pushableWheel = (PushableWheel*)sender->UserData;
  if(pushableWheel->PushKey->Key->State != pushableWheel->PushKey->ReleasedLevel) {
    pushableWheel->LastUpdatedItem = PressedWheel;
    pushableWheel->OnPressedWheelTicked(pushableWheel, direction);
  }
}

void PushableWheel_OnPushKeyStateChanged(GPIOKey *sender, BinaryKeyState state) {
  PushableWheel* pushableWheel = (PushableWheel*)sender->UserData;
  PushableWheelLastUpdatedItem lastUpdatedItem = pushableWheel->LastUpdatedItem;
  pushableWheel->LastUpdatedItem = PushKey;
  pushableWheel->OnPushKeyStateChanged(pushableWheel, state, lastUpdatedItem != PushKey);
}
