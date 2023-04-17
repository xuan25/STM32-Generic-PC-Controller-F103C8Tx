#ifndef __GPIO_PIN_H
#define __GPIO_PIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/**
 * @brief GPIO_Pin Structure definition
 * 
 * @note Used to define a GPIO pin
*/
typedef struct GPIO_Pin {
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
} GPIO_Pin;

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_PIN_H */
