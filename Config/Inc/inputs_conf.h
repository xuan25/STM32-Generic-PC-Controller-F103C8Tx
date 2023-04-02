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

typedef enum ActionType {
  ACTION_NONE = 0,
  ACTION_CTRL = 1,
  ACTION_KEYBOARD = 2,
  ACTION_MOUSE = 3,
  ACTION_RADIAL = 4,
  ACTION_MIDI = 5,
} ActionType;

typedef struct ActionConfig {
  ActionType Type;
  // ACTION_CTRL: lower byte
  // ACTION_KEYBOARD: modifier
  // ACTION_MOUSE: buttons
  // ACTION_RADIAL: button
  // ACTION_MIDI: cable number
  uint8_t Byte00;
  // ACTION_CTRL: higher byte
  // ACTION_KEYBOARD: OEM
  // ACTION_MOUSE: x
  // ACTION_RADIAL: dial lower byte
  // ACTION_MIDI: channel number
  uint8_t Byte01;
  // ACTION_KEYBOARD: key1
  // ACTION_MOUSE: y
  // ACTION_RADIAL: dial higher byte
  // ACTION_MIDI: controller number
  uint8_t Byte02;
  // ACTION_KEYBOARD: key2
  // ACTION_MOUSE: wheel
  // ACTION_RADIAL: x lower byte
  // ACTION_MIDI: change delta (0 for toggle)
  uint8_t Byte03;
  // ACTION_KEYBOARD: key3
  // ACTION_RADIAL: x higher byte
  uint8_t Byte04;
  // ACTION_KEYBOARD: key4
  // ACTION_RADIAL: y lower byte
  uint8_t Byte05;
  // ACTION_KEYBOARD: key5
  // ACTION_RADIAL: y higher byte
  uint8_t Byte06;
  // ACTION_KEYBOARD: key6
  // ACTION_RADIAL: width lower byte
  uint8_t Byte07;
  // ACTION_RADIAL: width higher byte
  uint8_t Byte08;
} ActionConfig;

extern ActionConfig actionConfigs[28];

void Inputs_Init();
void Inputs_Scan();

#ifdef __cplusplus
}
#endif

#endif /* __INPUTS_CONF_H */
