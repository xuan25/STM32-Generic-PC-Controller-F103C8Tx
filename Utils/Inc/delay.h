#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "tim.h"

// #define GPIO_GENERIC_DELAY_US 350
#define GPIO_GENERIC_DELAY_US 0

void Delay_Init();
void Delay_Us(uint16_t us);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */
