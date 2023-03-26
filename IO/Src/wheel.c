#include "wheel.h"

void Wheel_OnEncoderTick(struct Encoder* sender, int8_t direction, Encoder_Edge edge);
void Wheel_InputTick(Wheel* wheel, int8_t direction, Encoder_Edge edge);

void Wheel_Init(Wheel* wheel) {
  uint32_t tickMs = HAL_GetTick();
  wheel->LastInputTick = tickMs;
  wheel->InputState = 0;
  wheel->Encoder->UserData = wheel;
  wheel->Encoder->OnTicked = Wheel_OnEncoderTick;
  Encoder_Init(wheel->Encoder);
}

void Wheel_Scan(Wheel* wheel) {
  Encoder_Scan(wheel->Encoder);
}

void Wheel_OnEncoderTick(struct Encoder* sender, int8_t direction, Encoder_Edge edge) {
  Wheel* wheel = ((Wheel*)sender->UserData);
  Wheel_InputTick(wheel, direction, edge);
}

void Wheel_InputTick(Wheel* wheel, int8_t direction, Encoder_Edge edge) {
  // Check for reset delay 
  uint32_t currentTick = HAL_GetTick();
  if(wheel->ResetDelayMs > 0 && currentTick - wheel->LastInputTick > wheel->ResetDelayMs){
    wheel->InputState = 0;
  }
  wheel->LastInputTick = currentTick;

  // Check for tick interval
  if (edge & wheel->IgnoreInputEdge) {
    return;
  }
  wheel->InputState += direction;
  if (wheel->InputState > wheel->TickIntervalMs || wheel->InputState < -wheel->TickIntervalMs) {
    wheel->OnTicked(wheel, wheel->InputState);
    wheel->InputState = 0;
  }
}
