#include "delay.h"

void Delay_Init()
{
	HAL_TIM_Base_Start(&htim4);
}

void Delay_Us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim4, 0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim4) < us);  // wait for the counter to reach the us input in the parameter
}
