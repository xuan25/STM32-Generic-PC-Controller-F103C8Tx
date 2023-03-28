#include "ws2812b.h"
#include <math.h>

void WS2812BSeries_Init(WS2812BSeries* series) {
  series->Internal.CurrentCycle = 0;
  series->Internal.IsUpdating = 0;
  series->Internal.IsDirty = 0;

  uint16_t i;
  for (i = 0; series->Series[i] != NULL; i++) {
    WS2812B* unit = series->Series[i];
    unit->Internal.Parent = series;
  }
  series->Internal.NumUnits = i;
}

void WS2812BSeries_PushUpdate(WS2812BSeries* series) {
  if(series->Internal.IsUpdating) {
    // If a update pushing is ongoing, set the dirty flag and return.
    series->Internal.IsDirty = 1;
    return;
  }
  // start a new update pushing
  series->Internal.CurrentCycle = 0;
  series->Internal.IsUpdating = 1;
  series->Internal.IsDirty = 0;
  HAL_TIM_PWM_Start_DMA(series->TIM, series->TIMChannel, (uint32_t *)series->Internal.DMABuffer, WS2812B_DMA_BUFFER_LENGTH);
}

void WS2812BSeries_FillBuffer(WS2812BSeries* series, uint8_t bufferId) {
  if (series->Internal.CurrentCycle < series->Internal.NumUnits) {
    // fill buffer with a WS2812B unit
    uint16_t unitIdx = series->Internal.CurrentCycle;
    uint32_t arr = series->TIM->Instance->ARR + 1;
    uint32_t pulseHigh = (uint32_t)(arr * WS2812B_CODE_ONE_DUTY);
    uint32_t pulseLow = (uint32_t)(arr * WS2812B_CODE_ZERO_DUTY);

    WS2812B* unit = series->Series[unitIdx];
    uint16_t* buffer = &(series->Internal.DMABuffer[bufferId * (WS2812B_DMA_BUFFER_LENGTH / 2)]);
    for (size_t i = 0; i < 8; i++) {
      buffer[i] =        (unit->G >> (7 - i) & 0b1) ? pulseHigh : pulseLow;
      buffer[8 + i] =    (unit->R >> (7 - i) & 0b1) ? pulseHigh : pulseLow;
      buffer[16 + i] =   (unit->B >> (7 - i) & 0b1) ? pulseHigh : pulseLow;
    }
  } else if (series->Internal.CurrentCycle < (series->Internal.NumUnits + WS2812B_RESET_CYCLES)) {
    // fill reset
    if (series->Internal.CurrentCycle < (series->Internal.NumUnits + WS2812B_RESET_CYCLES + 2)) {
      memset(&series->Internal.DMABuffer[bufferId * (WS2812B_DMA_BUFFER_LENGTH / 2)], 0x00, (sizeof(series->Internal.DMABuffer) / 2));
    }
  } else {
    if(series->Internal.IsDirty) {
      // if the pushing is dirty, start over again
      series->Internal.IsDirty = 0;
      series->Internal.CurrentCycle = 0;
      return;
    }
    // stop DMA and exit
    HAL_TIM_PWM_Stop_DMA(series->TIM, series->TIMChannel);
    series->Internal.IsUpdating = 0;
  }
  
  series->Internal.CurrentCycle++;
}

void WS2812BSeries_OnHT(WS2812BSeries* series) {
  if(series->Internal.IsUpdating) {
    // The first buffer has been transmitted, refill it.
    WS2812BSeries_FillBuffer(series, 0);
  }
}

void WS2812BSeries_OnTC(WS2812BSeries* series) {
  if(series->Internal.IsUpdating) { 
    // The second buffer has been transmitted, refill it.
    WS2812BSeries_FillBuffer(series, 1);
  }
}

void WS2812B_SetHSV(WS2812B* unit, double h, double s, double v) {
	double r = 0, g = 0, b = 0;

	if (s == 0)
	{
		r = v;
		g = v;
		b = v;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (h == 360)
			h = 0;
		else
			h = h / 60;

		i = (int)trunc(h);
		f = h - i;

		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = v;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = v;
			b = p;
			break;

		case 2:
			r = p;
			g = v;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = v;
			break;

		case 4:
			r = t;
			g = p;
			b = v;
			break;

		default:
			r = v;
			g = p;
			b = q;
			break;
		}

	}

  unit->R = (uint8_t)(r * 255);
  unit->G = (uint8_t)(g * 255);
  unit->B = (uint8_t)(b * 255);
}