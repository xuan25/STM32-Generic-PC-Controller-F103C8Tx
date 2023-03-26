#ifndef __KEYMAT_H
#define __KEYMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "key.h"

typedef struct MatKey {
  void (*UserData);
  Key (*Key);
  uint16_t X;
  uint16_t Y;
} MatKey;

#define KEYMAT_ROWS 4u
#define KEYMAT_COLS 4u

typedef struct GPIO_Pin {
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
} GPIO_Pin;

void Keymat_Init(GPIO_Pin rows[], GPIO_Pin cols[], MatKey* matkeys[], uint8_t num_keys);
void Keymat_Scan();

#ifdef __cplusplus
}
#endif

#endif /* __KEYMAT_H */
