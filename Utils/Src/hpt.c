/**
  ******************************************************************************
  * @file    hpt.c
  * @brief   This file provides code for High Precision Timing with SysTick timer
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Xuan25.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "hpt.h"

extern __IO uint32_t uwTick;

/**
 * @brief Get the milliseconds from system initialization to the present moment
 * 
 * @return milliseconds
 * 
*/
uint32_t HPT_GetMs()
{
	return uwTick;
}

/**
 * @brief Get the microseconds from system initialization to the present moment
 * 
 * @return microseconds
 * 
*/
uint64_t HPT_GetUs()
{
	uint32_t msTotal, usPart;
	do {
		msTotal = uwTick;
		usPart = 1000U * (SysTick->LOAD - SysTick->VAL) / (SysTick->LOAD + 1);
	}
	while (msTotal != uwTick);	// read both ms and us part. Re-read if SysTick->VAL reloaded during the process.

	uint64_t usTotal = msTotal * 1000U + usPart;

	return usTotal;
}

/**
 * @brief Delay by milliseconds
 * 
 * @param us milliseconds
 * 
*/
void HPT_DelayMs(uint32_t ms)
{
	uint64_t startingMs = HPT_GetMs();
	uint64_t targetMs = startingMs + ms;
	while (HPT_GetMs() < targetMs);
}

/**
 * @brief Delay by microseconds
 * 
 * @param us microseconds
 * 
*/
void HPT_DelayUs(uint64_t us)
{
	uint64_t startingUs = HPT_GetUs();
	uint64_t targetUs = startingUs + us;
	while (HPT_GetUs() < targetUs);
}