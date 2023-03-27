#include "wheel.h"

void Wheel_OnEncoderTick(struct Encoder* sender, int8_t direction, Encoder_Edge edge);
void Wheel_InputTick(Wheel* wheel, int8_t direction, Encoder_Edge edge);

void Wheel_Init(Wheel* wheel) {
  uint32_t tickMs = HAL_GetTick();
  wheel->Internal.LastInputTickMs = tickMs;
  wheel->Internal.InputState = 0;
  wheel->Encoder->Internal.Parent = wheel;
  wheel->Encoder->OnTicked = Wheel_OnEncoderTick;
  Encoder_Init(wheel->Encoder);
}

void Wheel_Scan(Wheel* wheel) {
  Encoder_Scan(wheel->Encoder);
}

void Wheel_OnEncoderTick(struct Encoder* sender, int8_t direction, Encoder_Edge edge) {
  Wheel* wheel = ((Wheel*)sender->Internal.Parent);
  Wheel_InputTick(wheel, direction, edge);
}

void Wheel_InputTick(Wheel* wheel, int8_t direction, Encoder_Edge edge) {
  // Check for reset delay 
  uint32_t currentTick = HAL_GetTick();
  if(wheel->ResetDelayMs > 0 && currentTick - wheel->Internal.LastInputTickMs > wheel->ResetDelayMs){
    wheel->Internal.InputState = 0;
  }
  wheel->Internal.LastInputTickMs = currentTick;

  // Check for tick interval
  if (edge & wheel->IgnoreInputEdge) {
    return;
  }
  wheel->Internal.InputState += direction;
  if (wheel->Internal.InputState > wheel->TickInterval || wheel->Internal.InputState < -wheel->TickInterval) {
    wheel->OnTicked(wheel, wheel->Internal.InputState);
    wheel->Internal.InputState = 0;
  }
}
