#include "lighting_conf.h"
#include "tim.h"
#include "stm32f1xx_hal.h"

uint8_t matrixKeyLightMapping[] = {
  0, 0, 1, 2, 
  3, 4, 5, 6, 
  7, 8, 0, 0, 
  9, 10, 0, 0
};

uint8_t dialLightMapping[] = {
  0, 0
};

// channelID | controllerID << 8
uint16_t lightMIDICCMappings[] = {
  0x0 | 80 << 8,
  0x0 | 81 << 8,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
};

RGB rgbIndex[] = {
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // dial 0 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // dial 1 pressed brighter white
  { .R = 0x52 * 0.8, .G = 0xf7 * 0.8, .B = 0xf1 * 0.8 },  // dial 0 pressed CW blue
  { .R = 0x52 * 0.8, .G = 0xf7 * 0.8, .B = 0xf1 * 0.8 },  // dial 1 pressed CW blue
  { .R = 0xf7 * 0.8, .G = 0xd6 * 0.8, .B = 0x39 * 0.8 },  // dial 0 pressed CCW yellow
  { .R = 0xf7 * 0.8, .G = 0xd6 * 0.8, .B = 0x39 * 0.8 },  // dial 1 pressed CCW yellow
  { .R = 0xfa * 0.8, .G = 0x49 * 0.8, .B = 0xd6 * 0.8},   // dial 0 released pink
  { .R = 0xfa * 0.8, .G = 0x49 * 0.8, .B = 0xd6 * 0.8},   // dial 1 released pink
  { .R = 0x52 * 0.8, .G = 0xf7 * 0.8, .B = 0xf1 * 0.8 },  // dial 0 released CW blue
  { .R = 0x52 * 0.8, .G = 0xf7 * 0.8, .B = 0xf1 * 0.8 },  // dial 1 released CW blue
  { .R = 0xf7 * 0.8, .G = 0xd6 * 0.8, .B = 0x39 * 0.8 },  // dial 0 released CCW yellow
  { .R = 0xf7 * 0.8, .G = 0xd6 * 0.8, .B = 0x39 * 0.8 },  // dial 1 released CCW yellow

  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 00 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 01 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 02 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 03 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 04 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 05 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 06 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 07 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 08 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 09 default dim white
  { .R = 0xff * 0.2, .G = 0xff * 0.2, .B = 0xff * 0.2 },  // key 10 default dim white

  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 00 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 01 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 02 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 03 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 04 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 05 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 06 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 07 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 08 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 09 pressed brighter white
  { .R = 0xff * 0.5, .G = 0xff * 0.5, .B = 0xff * 0.5 },  // key 10 pressed brighter white

  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 00 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 01 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 02 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 03 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 04 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 05 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 06 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 07 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 08 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 09 Activated
  { .R = 0xff, .G = 0x00, .B = 0x00 },                    // key 10 Activated
  
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 00 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 01 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 02 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 03 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 04 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 05 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 06 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 07 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 08 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 09 temp black
  { .R = 0x00, .G = 0x00, .B = 0x00 },                    // key 10 temp black
};

WS2812BSeries *ws2812bSeries = &(WS2812BSeries) {
  .Series = (WS2812B *[]){
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_00_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_00_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_01_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_01_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_02_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_02_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_03_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_03_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_04_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_04_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_05_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_05_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_06_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_06_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_07_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_07_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_08_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_08_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_09_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_09_TEMP
              }
            }
          }
        }
      }
    },
    &(WS2812B) {
      .Color = &(Color) {
        .RGB = RGB_KEY_10_DEFAULT,
        .Filter = &(Filter){
          .Function = AlphaFilter,
          .Params = &(AlphaFilterParams) {
            .Alpha = ALPHA_MAX
          },
          .Next = &(Filter){
            .Function = EasingFilter,
            .Params = &(EasingFilterParams) {
              .BeginTime = 0,
              .Duration = 1000,
              .EasingFrom = &(Color) {
                .RGB = RGB_KEY_10_TEMP
              }
            }
          }
        }
      }
    },
    NULL
  },
  .TIM = &htim2,
  .TIMChannel = TIM_CHANNEL_4
};

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  WS2812BSeries_OnHT(ws2812bSeries);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  WS2812BSeries_OnTC(ws2812bSeries);
}

void Lighting_Init() {
  WS2812BSeries_Init(ws2812bSeries);
}

void Lighting_PushUpdate() {
  WS2812BSeries_PushUpdate(ws2812bSeries);
}

void Lighting_OnKeyMatrixStateChanged(uint8_t keyID, BinaryPushKeyState state) {
  uint8_t lightId = matrixKeyLightMapping[keyID];
  Color *easingTo = ws2812bSeries->Series[lightId]->Color;
  AlphaFilterParams *alphaParams = (AlphaFilterParams *)ws2812bSeries->Series[lightId]->Color->Filter->Params;
  EasingFilterParams *easingParams = (EasingFilterParams *)ws2812bSeries->Series[lightId]->Color->Filter->Next->Params;
  Color *easingFrom = easingParams->EasingFrom;
  // evaluate color under current config first
  *(RGB_KEY_00_TEMP + lightId) = Color_EvaluateRGB(easingTo);
  // then update config
  easingFrom->RGB = RGB_KEY_00_TEMP + lightId;
  if (easingTo->RGB != RGB_KEY_00_ACTIVATED + lightId) {
    if(state == PushKeyPressed) {
      easingTo->RGB = RGB_KEY_00_PRESSED + lightId;
    } 
    else {
      easingTo->RGB = RGB_KEY_00_DEFAULT + lightId;
    }
  }

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = LIGHT_EASING_DURATION;
}

void Lighting_OnDialReleasedTicked(uint8_t dialID, int8_t direction) {
  uint8_t lightId = dialLightMapping[dialID];
  Color *easingTo = ws2812bSeries->Series[lightId]->Color;
  AlphaFilterParams *alphaParams = (AlphaFilterParams *)ws2812bSeries->Series[lightId]->Color->Filter->Params;
  EasingFilterParams *easingParams = (EasingFilterParams *)ws2812bSeries->Series[lightId]->Color->Filter->Next->Params;
  Color *easingFrom = easingParams->EasingFrom;

  if(direction > 0) {
    easingFrom->RGB = RGB_DIAL_1_RELEASED_CW + dialID;
  } else {
    easingFrom->RGB = RGB_DIAL_1_RELEASED_CCW + dialID;
  }

  // easingTo->RGB = RGB_KEY_00_DEFAULT + lightId;

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = LIGHT_EASING_DURATION;
}

void Lighting_OnDialPressedTicked(uint8_t dialID, int8_t direction) {
  uint8_t lightId = dialLightMapping[dialID];
  Color *easingTo = ws2812bSeries->Series[lightId]->Color;
  AlphaFilterParams *alphaParams = (AlphaFilterParams*)ws2812bSeries->Series[lightId]->Color->Filter->Params;
  EasingFilterParams *easingParams = (EasingFilterParams*)ws2812bSeries->Series[lightId]->Color->Filter->Next->Params;
  Color *easingFrom = easingParams->EasingFrom;

  if(direction > 0) {
    easingFrom->RGB = RGB_DIAL_1_PRESSED_CW + dialID;
  } else {
    easingFrom->RGB = RGB_DIAL_1_PRESSED_CCW + dialID;
  }

  // easingTo->RGB = RGB_DIAL_1_PRESSED + dialID;

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = LIGHT_EASING_DURATION;
}

void Lighting_OnDialKeyStateChanged(uint8_t dialID, BinaryPushKeyState state, uint8_t isDialTicked) {
  uint8_t lightId = dialLightMapping[dialID];
  Color *easingTo = ws2812bSeries->Series[lightId]->Color;
  AlphaFilterParams *alphaParams = (AlphaFilterParams*)ws2812bSeries->Series[lightId]->Color->Filter->Params;
  EasingFilterParams *easingParams = (EasingFilterParams*)ws2812bSeries->Series[lightId]->Color->Filter->Next->Params;
  Color *easingFrom = easingParams->EasingFrom;

  if(state == PushKeyPressed) {
    // evaluate color under current config first
    *(RGB_KEY_00_TEMP + lightId) = Color_EvaluateRGB(easingTo);
    // then update config
    easingFrom->RGB = RGB_KEY_00_TEMP + lightId;
    if (easingTo->RGB != RGB_KEY_00_ACTIVATED + lightId) {
      easingTo->RGB = RGB_DIAL_1_PRESSED + dialID;
    }
  } else {
    if (!isDialTicked) {
      easingFrom->RGB = RGB_DIAL_1_RELEASED + dialID;
      if (easingTo->RGB != RGB_KEY_00_ACTIVATED + lightId) {
        easingTo->RGB = RGB_KEY_00_DEFAULT + lightId;
      }
    } else {
      // evaluate color under current config first
      *(RGB_KEY_00_TEMP + lightId) = Color_EvaluateRGB(easingTo);
      // then update config
      easingFrom->RGB = RGB_KEY_00_TEMP + lightId;
      if (easingTo->RGB != RGB_KEY_00_ACTIVATED + lightId) {
        easingTo->RGB = RGB_KEY_00_DEFAULT + lightId;
      }
    }
  }

  easingParams->BeginTime = HAL_GetTick();
  easingParams->IsCompleted = 0;
  easingParams->Duration = LIGHT_EASING_DURATION;
}

void Lighting_OnMIDICCChanged(uint8_t channelNumber, uint8_t controllerNumber, uint8_t value) {
  uint32_t midiCCMapID = channelNumber | controllerNumber << 8;

  for (uint8_t lightId = 0; lightId < sizeof(lightMIDICCMappings); lightId++) {
    if (lightMIDICCMappings[lightId] == midiCCMapID) {
      
      Color *easingTo = ws2812bSeries->Series[lightId]->Color;
      AlphaFilterParams *alphaParams = (AlphaFilterParams*)ws2812bSeries->Series[lightId]->Color->Filter->Params;
      EasingFilterParams *easingParams = (EasingFilterParams*)ws2812bSeries->Series[lightId]->Color->Filter->Next->Params;
      Color *easingFrom = easingParams->EasingFrom;

      // evaluate color under current config first
      *(RGB_KEY_00_TEMP + lightId) = Color_EvaluateRGB(easingTo);
      // then update config
      easingFrom->RGB = RGB_KEY_00_TEMP + lightId;
      
      if (value > 63) {
        easingTo->RGB = RGB_KEY_00_ACTIVATED + lightId;
      } else {
        easingTo->RGB = RGB_KEY_00_DEFAULT + lightId;
      }

      easingParams->BeginTime = HAL_GetTick();
      easingParams->IsCompleted = 0;
      easingParams->Duration = LIGHT_EASING_DURATION;

      break;
    }
  }
}