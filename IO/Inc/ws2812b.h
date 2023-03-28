#ifndef __WS2812B_H
#define __WS2812B_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include <string.h>

#define WS2812B_RESET_CYCLES 2u
#define WS2812B_CODE_ONE_DUTY	  2/3.0
#define WS2812B_CODE_ZERO_DUTY	1/3.0

#define WS2812B_BYTES_PER_UNIT 3

#define WS2812B_DMA_BUFFER_LENGTH   2 * WS2812B_BYTES_PER_UNIT * 8


typedef struct WS2812B_Internal {
  void (*Parent);     // Parent
} WS2812B_Internal;

/**
 * @brief WS2812B Structure definition
 * 
 * @note Used to managed a unit of WS2812B.
*/
typedef struct WS2812B {
  WS2812B_Internal Internal;  // For internal usage
  uint8_t R;                  // Red Channel
  uint8_t G;                  // Green Channel
  uint8_t B;                  // Blue Channel
} WS2812B;

typedef struct WS2812BSeries_Internal {
  void (*Parent);                                   // Parent
  uint16_t NumUnits;                                // Number of Units
  uint8_t CurrentCycle;                             // Current update pushing cycle
  uint8_t IsUpdating;                               // Whether an updating push is ongoing 
  uint16_t DMABuffer[WS2812B_DMA_BUFFER_LENGTH];    // Double-buffer for DMA
  uint8_t IsDirty;                                  // Whether have changes that need to be pushed
} WS2812BSeries_Internal;

/**
 * @brief WS2812BSeries Structure definition
 * 
 * @note Used to managed a Series of WS2812B units.
*/
typedef struct WS2812BSeries {
  WS2812BSeries_Internal Internal;    // For internal usage
  WS2812B** Series;                   // A null-terminated array of WS2812B units
  TIM_HandleTypeDef* TIM;             // Timer handle
  uint32_t TIMChannel;                // Timer channel
} WS2812BSeries;


/**
 * @brief Set color of a WS2812B unit with HSV
 * 
 * @param unit The WS2812B unit to be configure
 * @param h H
 * @param s S
 * @param v V
 * @retval None
*/
void WS2812B_SetHSV(WS2812B* unit, double h, double s, double v);

/**
 * @brief Initialize the WS2812BSeries
 * 
 * @param series The WS2812BSeries to be initialized
 * @retval None
*/
void WS2812BSeries_Init(WS2812BSeries* series);

/**
 * @brief Push Update form WS2812BSeries structure to hardware
 * 
 * @param series The WS2812BSeries to be pushed
 * @retval None
*/
void WS2812BSeries_PushUpdate(WS2812BSeries* series);

/**
 * @brief Fill DMA buffer
 * 
 * @param series The WS2812BSeries to be processed
 * @param bufferId The ID of the buffer to be filled
 * @retval None
*/
void WS2812BSeries_FillBuffer(WS2812BSeries* series, uint8_t bufferId);

/**
 * @brief Handle HT event of DMA
 * 
 * @param series The WS2812BSeries that associated to the event
 * @retval None
*/
void WS2812BSeries_OnHT(WS2812BSeries* series);

/**
 * @brief Handle TC event of DMA
 * 
 * @param series The WS2812BSeries that associated to the event
 * @retval None
*/
void WS2812BSeries_OnTC(WS2812BSeries* series);


#ifdef __cplusplus
}
#endif

#endif /* __WS2812B_H */
