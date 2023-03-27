#include "pushable_dial.h"

void PushableDial_OnReleasedDialTicked(Dial *sender, int8_t direction);
void PushableDial_OnPressedDialTicked(Dial *sender, int8_t direction);
void PushableDial_OnPushKeyStateChanged(BinaryPushKey *sender, BinaryPushKeyState state);

void PushableDial_Init(PushableDial* pushableDial) {
  if(pushableDial->ReleasedDial != NULL) {
    pushableDial->ReleasedDial->Internal.Parent = pushableDial;
    pushableDial->ReleasedDial->OnTicked = PushableDial_OnReleasedDialTicked;
    Dial_Init(pushableDial->ReleasedDial);
  }
  
  if(pushableDial->ReleasedDial != NULL) {
    pushableDial->PressedDial->Internal.Parent = pushableDial;
    pushableDial->PressedDial->OnTicked = PushableDial_OnPressedDialTicked;
    Dial_Init(pushableDial->PressedDial);
  }

  pushableDial->PushKey->Internal.Parent = pushableDial;
  pushableDial->PushKey->OnStateChanged = PushableDial_OnPushKeyStateChanged;
  BinaryPushKey_Init(pushableDial->PushKey);
}

void PushableDial_Scan(PushableDial* pushableDial) {
  Dial_Scan(pushableDial->PressedDial);
  Dial_Scan(pushableDial->ReleasedDial);
  BinaryPushKey_Scan(pushableDial->PushKey);
}

void PushableDial_OnReleasedDialTicked(Dial *sender, int8_t direction) {
  PushableDial* pushableDial = (PushableDial*)sender->Internal.Parent;
  if(pushableDial->PushKey->Key->Internal.State == pushableDial->PushKey->ReleasedLevel) {
    pushableDial->Internal.LastUpdatedItem = ReleasedDial;
    pushableDial->OnReleasedDialTicked(pushableDial, direction);
  }
}

void PushableDial_OnPressedDialTicked(Dial *sender, int8_t direction){
  PushableDial* pushableDial = (PushableDial*)sender->Internal.Parent;
  if(pushableDial->PushKey->Key->Internal.State != pushableDial->PushKey->ReleasedLevel) {
    pushableDial->Internal.LastUpdatedItem = PressedDial;
    pushableDial->OnPressedDialTicked(pushableDial, direction);
  }
}

void PushableDial_OnPushKeyStateChanged(BinaryPushKey *sender, BinaryPushKeyState state) {
  PushableDial* pushableDial = (PushableDial*)sender->Internal.Parent;
  PushableDialLastUpdatedItem lastUpdatedItem = pushableDial->Internal.LastUpdatedItem;
  pushableDial->Internal.LastUpdatedItem = PushKey;
  pushableDial->OnPushKeyStateChanged(pushableDial, state, lastUpdatedItem != PushKey);
}
