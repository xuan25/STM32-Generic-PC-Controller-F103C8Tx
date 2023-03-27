#include "pushable_wheel.h"

void PushableWheel_OnReleasedWheelTicked(Wheel *sender, int8_t direction);
void PushableWheel_OnPressedWheelTicked(Wheel *sender, int8_t direction);
void PushableWheel_OnPushKeyStateChanged(BinaryPushKey *sender, BinaryPushKeyState state);

void PushableWheel_Init(PushableWheel* pushableWheel) {
  if(pushableWheel->ReleasedWheel != NULL) {
    pushableWheel->ReleasedWheel->Internal.Parent = pushableWheel;
    pushableWheel->ReleasedWheel->OnTicked = PushableWheel_OnReleasedWheelTicked;
    Wheel_Init(pushableWheel->ReleasedWheel);
  }
  
  if(pushableWheel->ReleasedWheel != NULL) {
    pushableWheel->PressedWheel->Internal.Parent = pushableWheel;
    pushableWheel->PressedWheel->OnTicked = PushableWheel_OnPressedWheelTicked;
    Wheel_Init(pushableWheel->PressedWheel);
  }

  pushableWheel->PushKey->Internal.Parent = pushableWheel;
  pushableWheel->PushKey->OnStateChanged = PushableWheel_OnPushKeyStateChanged;
  BinaryPushKey_Init(pushableWheel->PushKey);
}

void PushableWheel_Scan(PushableWheel* pushableWheel) {
  Wheel_Scan(pushableWheel->PressedWheel);
  Wheel_Scan(pushableWheel->ReleasedWheel);
  BinaryPushKey_Scan(pushableWheel->PushKey);
}

void PushableWheel_OnReleasedWheelTicked(Wheel *sender, int8_t direction) {
  PushableWheel* pushableWheel = (PushableWheel*)sender->Internal.Parent;
  if(pushableWheel->PushKey->Key->Internal.State == pushableWheel->PushKey->ReleasedLevel) {
    pushableWheel->Internal.LastUpdatedItem = ReleasedWheel;
    pushableWheel->OnReleasedWheelTicked(pushableWheel, direction);
  }
}

void PushableWheel_OnPressedWheelTicked(Wheel *sender, int8_t direction){
  PushableWheel* pushableWheel = (PushableWheel*)sender->Internal.Parent;
  if(pushableWheel->PushKey->Key->Internal.State != pushableWheel->PushKey->ReleasedLevel) {
    pushableWheel->Internal.LastUpdatedItem = PressedWheel;
    pushableWheel->OnPressedWheelTicked(pushableWheel, direction);
  }
}

void PushableWheel_OnPushKeyStateChanged(BinaryPushKey *sender, BinaryPushKeyState state) {
  PushableWheel* pushableWheel = (PushableWheel*)sender->Internal.Parent;
  PushableWheelLastUpdatedItem lastUpdatedItem = pushableWheel->Internal.LastUpdatedItem;
  pushableWheel->Internal.LastUpdatedItem = PushKey;
  pushableWheel->OnPushKeyStateChanged(pushableWheel, state, lastUpdatedItem != PushKey);
}
