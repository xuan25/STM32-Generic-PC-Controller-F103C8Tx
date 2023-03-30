#include "key.h"

void Key_Init(Key* key, uint8_t level) {
  key->Internal.LastChangedLevel = level;
  #if KEY_DEBOUNCE_MS > 0
    uint32_t tickMs = HAL_GetTick();
    key->Internal.LastLevelChangedMs = tickMs;
  #endif
}

void Key_Update(Key* key, uint8_t level) {
#if KEY_DEBOUNCE_MS > 0
  uint32_t tickMs = HAL_GetTick();
  // Level update
  if(key->Internal.LastChangedLevel != level) {
    key->Internal.LastChangedLevel = level;
    key->Internal.LastLevelChangedMs = tickMs;
  }
  // State update
  if(key->Internal.State != level && tickMs - key->Internal.LastLevelChangedMs > KEY_DEBOUNCE_MS) {
    uint8_t oldState = key->Internal.State;
    key->Internal.State = level;
    if(key->OnStateChanged == NULL || !key->OnStateChanged(key, oldState, level)) {
      if(key->Internal.OnStateChanged != NULL) {
        key->Internal.OnStateChanged(key, oldState, level);
      }
    }
  }
#else
  // State update
  if(key->State != level) {
    uint8_t oldState = key->State;
    key->State = level;
    if(key->OnStateChanged == NULL || !key->OnStateChanged(key, oldState, level)) {
      if(key->Internal.OnStateChanged != NULL) {
        key->Internal.OnStateChanged(key, oldState, level);
      }
    }
  }
#endif
}
