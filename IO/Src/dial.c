#include "dial.h"

void Dial_OnEncoderTick(struct Encoder* sender, int8_t direction, Encoder_Edge edge);
void Dial_InputTick(Dial* dial, int8_t direction, Encoder_Edge edge);

void Dial_Init(Dial* dial) {
  uint32_t tickMs = HAL_GetTick();
  dial->Internal.LastInputTickMs = tickMs;
  dial->Internal.InputState = 0;
  dial->Encoder->Internal.Parent = dial;
  dial->Encoder->OnTicked = Dial_OnEncoderTick;
  Encoder_Init(dial->Encoder);
}

void Dial_Scan(Dial* dial) {
  Encoder_Scan(dial->Encoder);
}

void Dial_OnEncoderTick(struct Encoder* sender, int8_t direction, Encoder_Edge edge) {
  Dial* dial = ((Dial*)sender->Internal.Parent);
  Dial_InputTick(dial, direction, edge);
}

void Dial_InputTick(Dial* dial, int8_t direction, Encoder_Edge edge) {
  // Check for reset delay 
  uint32_t currentTick = HAL_GetTick();
  if(dial->ResetDelayMs > 0 && currentTick - dial->Internal.LastInputTickMs > dial->ResetDelayMs){
    dial->Internal.InputState = 0;
  }
  dial->Internal.LastInputTickMs = currentTick;

  // Check for tick interval
  if (edge & dial->IgnoreInputEdge) {
    return;
  }
  dial->Internal.InputState += direction;
  if (dial->Internal.InputState > dial->TickInterval || dial->Internal.InputState < -dial->TickInterval) {
    dial->OnTicked(dial, dial->Internal.InputState);
    dial->Internal.InputState = 0;
  }
}