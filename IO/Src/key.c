#include "key.h"

void Key_Init(Key* key, uint8_t level) {
  key->LastChangedLevel = level;
  #if KEY_DEBOUNCE_MS > 0
    uint32_t tickMs = HAL_GetTick();
    key->LastLevelChangedMs = tickMs;
  #endif
}

void Key_Update(Key* key, uint8_t level) {
#if KEY_DEBOUNCE_MS > 0
  uint32_t tickMs = HAL_GetTick();
  // Level update
  if(key->LastChangedLevel != level) {
    key->LastChangedLevel = level;
    key->LastLevelChangedMs = tickMs;
  }
  // State update
  if(key->State != level && tickMs - key->LastLevelChangedMs > KEY_DEBOUNCE_MS) {
    uint8_t oldState = key->State;
    key->State = level;
    (*key->OnStateChanged)(key, oldState, level);
  }
#else
  // State update
  if(key->State != level) {
    uint8_t oldState = key->State;
    key->State = level;
    (*key->OnStateChanged)(key, oldState, level);
  }
#endif
}
