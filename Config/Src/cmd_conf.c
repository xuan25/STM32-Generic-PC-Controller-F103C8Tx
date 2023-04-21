/**
  ******************************************************************************
  * @file    cmd_conf.c
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

#include "cmd_conf.h"
#include "usbd_hid_if.h"
#include "inputs_conf.h"
#include "lighting_conf.h"
#include "flash_conf.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t returnBuffer[VENDER_DATA_LENGTH];

uint8_t *CMD_Exec(uint8_t *cmd) {
  switch (cmd[0])
  {
  case 'A':
    {
      // action
      switch (cmd[1])
      {
      case 'W':
        {
          // action write
          uint8_t id = cmd[2];
          ActionConfig *dest = actionConfigs + id;
          uint8_t *src = &cmd[3];
          memcpy(dest, src, sizeof(ActionConfig));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // action read
          uint8_t id = cmd[2];
          ActionConfig *src = actionConfigs + id;
          strcpy(returnBuffer, "S\0");
          uint8_t *dest = &returnBuffer[2];
          memcpy(dest, src, sizeof(ActionConfig));
        }
        break;
      default:
        strcpy(returnBuffer, "E: Unknown command for action");
        break;
      }
    }
    break;
  case 'L':
    {
      // lighting
      switch (cmd[1])
      {
      case 'W':
        {
          // lighting write
          uint8_t id = cmd[2];
          RGB *dest = rgbIndex + id;
          uint8_t *src = &cmd[3];
          memcpy(dest, src, sizeof(RGB));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // lighting read
          uint8_t id = cmd[2];
          RGB *src = rgbIndex + id;
          strcpy(returnBuffer, "S\0");
          uint8_t *dest = &returnBuffer[2];
          memcpy(dest, src, sizeof(RGB));
        }
        break;
      default:
        strcpy(returnBuffer, "E: Unknown command for lighting");
        break;
      }
    }
    break;
   case 'D':
    {
      // dial-light mapping
      switch (cmd[1])
      {
      case 'W':
        {
          // dial-light mapping write
          uint8_t id = cmd[2];
          uint16_t *dest = dialLightMapping + id;
          uint8_t *src = &cmd[3];
          memcpy(dest, src, sizeof(uint16_t));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // dial-light mapping read
          uint8_t id = cmd[2];
          uint16_t *src = dialLightMapping + id;
          strcpy(returnBuffer, "S\0");
          uint8_t *dest = &returnBuffer[2];
          memcpy(dest, src, sizeof(uint16_t));
        }
        break;
      default:
        strcpy(returnBuffer, "E: Unknown command for dial-light mapping");
        break;
      }
    }
    break;
  case 'M':
    {
      // light-MIDICC mapping
      switch (cmd[1])
      {
      case 'W':
        {
          // light-MIDICC mapping write
          uint8_t id = cmd[2];
          uint16_t *dest = lightMIDICCMappings + id;
          uint8_t *src = &cmd[3];
          memcpy(dest, src, sizeof(uint16_t));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // light-MIDICC mapping read
          uint8_t id = cmd[2];
          uint16_t *src = lightMIDICCMappings + id;
          strcpy(returnBuffer, "S\0");
          uint8_t *dest = &returnBuffer[2];
          memcpy(dest, src, sizeof(uint16_t));
        }
        break;
      default:
        strcpy(returnBuffer, "E: Unknown command for light-MIDICC mapping");
        break;
      }
    }
    break;
  case 'S':
    {
      // save
      FlashConfig_Save();
      strcpy(returnBuffer, "S: OK");
    }
    break;
  case 'R':
    {
      // reset
      FlashConfig_Reset();
      strcpy(returnBuffer, "S: OK");
    }
    break;
  default:
    strcpy(returnBuffer, "E: Unknown command");
    break;
  }

  return returnBuffer;
}

int8_t HID_OutEvent_FS(uint8_t *buffer)
{
  /* USER CODE BEGIN 6 */
  uint8_t reportId = buffer[0];
  switch (reportId)
  {
  case VENDER_REPORT_ID:
  {
    uint8_t *returnBuffer = CMD_Exec(buffer + 1);
    uint8_t venderBuffer[VENDER_REPORT_LENGTH];
    venderBuffer[0] = VENDER_REPORT_ID;
    memcpy(venderBuffer + 1, returnBuffer, VENDER_DATA_LENGTH);
    while (USBD_HID_SendReport(&hUsbDeviceFS, venderBuffer, VENDER_REPORT_LENGTH) == USBD_BUSY);
    return (USBD_OK);
    break;
  }
  case LIGHTING_REPORT_ID:
  {
    uint8_t updateFlag = buffer[1] &  0xf;
    uint8_t updateCount = buffer[1] >> 4 & 0xf;
    for (int i = 0; i < updateCount; i++) {
      uint8_t id = buffer[0x02 + i];
      uint8_t r = buffer[0x11 + i];
      uint8_t g = buffer[0x20 + i];
      uint8_t b = buffer[0x2f + i];
      // TODO: update state buffer
      
      rgbIndex[id].R = r;
      rgbIndex[id].G = g;
      rgbIndex[id].B = b;
      
    }
    if (updateFlag == 1) {
      // TODO: push updates
    }
    return (USBD_OK);
    break;
  }
  default:
    break;
  }
  
  return (USBD_OK);
  /* USER CODE END 6 */
}
