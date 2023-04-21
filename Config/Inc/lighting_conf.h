/**
  ******************************************************************************
  * @file    lighting_conf.h
  * @brief   
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

#ifndef __LIGHTING_CONF_H
#define __LIGHTING_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ws2812b.h"
#include "binary_push_key.h"

#define LIGHT_EASING_DURATION    200

extern uint16_t dialLightMapping[2];
extern uint16_t lightMIDICCMappings[11];
extern RGB rgbIndex[56];



#define RGB_DIAL_1_PRESSED          (rgbIndex +  0)
#define RGB_DIAL_2_PRESSED          (rgbIndex +  1)
#define RGB_DIAL_1_PRESSED_CW       (rgbIndex +  2)
#define RGB_DIAL_2_PRESSED_CW       (rgbIndex +  3)
#define RGB_DIAL_1_PRESSED_CCW      (rgbIndex +  4)
#define RGB_DIAL_2_PRESSED_CCW      (rgbIndex +  5)
#define RGB_DIAL_1_RELEASED         (rgbIndex +  6)
#define RGB_DIAL_2_RELEASED         (rgbIndex +  7)
#define RGB_DIAL_1_RELEASED_CW      (rgbIndex +  8)
#define RGB_DIAL_2_RELEASED_CW      (rgbIndex +  9)
#define RGB_DIAL_1_RELEASED_CCW     (rgbIndex + 10)
#define RGB_DIAL_2_RELEASED_CCW     (rgbIndex + 11)

#define RGB_KEY_00_DEFAULT          (rgbIndex + 12)
#define RGB_KEY_01_DEFAULT          (rgbIndex + 13)
#define RGB_KEY_02_DEFAULT          (rgbIndex + 14)
#define RGB_KEY_03_DEFAULT          (rgbIndex + 15)
#define RGB_KEY_04_DEFAULT          (rgbIndex + 16)
#define RGB_KEY_05_DEFAULT          (rgbIndex + 17)
#define RGB_KEY_06_DEFAULT          (rgbIndex + 18)
#define RGB_KEY_07_DEFAULT          (rgbIndex + 19)
#define RGB_KEY_08_DEFAULT          (rgbIndex + 20)
#define RGB_KEY_09_DEFAULT          (rgbIndex + 21)
#define RGB_KEY_10_DEFAULT          (rgbIndex + 22)

#define RGB_KEY_00_PRESSED          (rgbIndex + 23)
#define RGB_KEY_01_PRESSED          (rgbIndex + 24)
#define RGB_KEY_02_PRESSED          (rgbIndex + 25)
#define RGB_KEY_03_PRESSED          (rgbIndex + 26)
#define RGB_KEY_04_PRESSED          (rgbIndex + 27)
#define RGB_KEY_05_PRESSED          (rgbIndex + 28)
#define RGB_KEY_06_PRESSED          (rgbIndex + 29)
#define RGB_KEY_07_PRESSED          (rgbIndex + 30)
#define RGB_KEY_08_PRESSED          (rgbIndex + 31)
#define RGB_KEY_09_PRESSED          (rgbIndex + 32)
#define RGB_KEY_10_PRESSED          (rgbIndex + 33)

#define RGB_KEY_00_ACTIVATED             (rgbIndex + 34)
#define RGB_KEY_01_ACTIVATED             (rgbIndex + 35)
#define RGB_KEY_02_ACTIVATED             (rgbIndex + 36)
#define RGB_KEY_03_ACTIVATED             (rgbIndex + 37)
#define RGB_KEY_04_ACTIVATED             (rgbIndex + 38)
#define RGB_KEY_05_ACTIVATED             (rgbIndex + 39)
#define RGB_KEY_06_ACTIVATED             (rgbIndex + 40)
#define RGB_KEY_07_ACTIVATED             (rgbIndex + 41)
#define RGB_KEY_08_ACTIVATED             (rgbIndex + 42)
#define RGB_KEY_09_ACTIVATED             (rgbIndex + 43)
#define RGB_KEY_10_ACTIVATED             (rgbIndex + 44)

#define RGB_KEY_00_TEMP        (rgbIndex + 45)
#define RGB_KEY_01_TEMP        (rgbIndex + 46)
#define RGB_KEY_02_TEMP        (rgbIndex + 47)
#define RGB_KEY_03_TEMP        (rgbIndex + 48)
#define RGB_KEY_04_TEMP        (rgbIndex + 49)
#define RGB_KEY_05_TEMP        (rgbIndex + 50)
#define RGB_KEY_06_TEMP        (rgbIndex + 51)
#define RGB_KEY_07_TEMP        (rgbIndex + 52)
#define RGB_KEY_08_TEMP        (rgbIndex + 53)
#define RGB_KEY_09_TEMP        (rgbIndex + 54)
#define RGB_KEY_10_TEMP        (rgbIndex + 55)

extern RGB rgbIndex[];

void Lighting_Init();
void Lighting_PushUpdate();
void Lighting_OnKeyMatrixStateChanged(uint8_t keyID, BinaryPushKeyState state);
void Lighting_OnDialReleasedTicked(uint8_t dialID, int8_t direction);
void Lighting_OnDialPressedTicked(uint8_t dialID, int8_t direction);
void Lighting_OnDialKeyStateChanged(uint8_t dialID, BinaryPushKeyState state, uint8_t isDialTicked);
void Lighting_OnMIDICCChanged(uint8_t channelNumber, uint8_t controllerNumber, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* __LIGHTING_CONF_H */
