#ifndef __INPUTS_CONF_H
#define __INPUTS_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define ACTION_DIAL_0_RELEASED_CW   (actionConfigs +  0)
#define ACTION_DIAL_1_RELEASED_CW   (actionConfigs +  1)
#define ACTION_DIAL_0_RELEASED_CCW  (actionConfigs +  2)
#define ACTION_DIAL_1_RELEASED_CCW  (actionConfigs +  3)
#define ACTION_DIAL_0_PRESSED_CW    (actionConfigs +  4)
#define ACTION_DIAL_1_PRESSED_CW    (actionConfigs +  5)
#define ACTION_DIAL_0_PRESSED_CCW   (actionConfigs +  6)
#define ACTION_DIAL_1_PRESSED_CCW   (actionConfigs +  7)
#define ACTION_DIAL_0_PUSH_KEY      (actionConfigs +  8)
#define ACTION_DIAL_1_PUSH_KEY      (actionConfigs +  9)
#define ACTION_DIAL_0_CLICKED       (actionConfigs + 10)
#define ACTION_DIAL_1_CLICKED       (actionConfigs + 11)
#define ACTION_MatrixKey_00         (actionConfigs + 12)
#define ACTION_MatrixKey_01         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 1])
#define ACTION_MatrixKey_02         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 2])
#define ACTION_MatrixKey_03         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 3])
#define ACTION_MatrixKey_04         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 4])
#define ACTION_MatrixKey_05         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 5])
#define ACTION_MatrixKey_06         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 6])
#define ACTION_MatrixKey_07         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 7])
#define ACTION_MatrixKey_08         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 8])
#define ACTION_MatrixKey_09         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[ 9])
#define ACTION_MatrixKey_10         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[10])
#define ACTION_MatrixKey_11         (ACTION_MatrixKey_00 + matrixKeyConfigsMapping[11])

void Inputs_Init();
void Inputs_Scan();

#ifdef __cplusplus
}
#endif

#endif /* __INPUTS_CONF_H */
