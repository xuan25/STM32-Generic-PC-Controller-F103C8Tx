#include "cmd_conf.h"
#include "usbd_hid_if.h"
#include "inputs_conf.h"
#include "lighting_conf.h"
#include "flash_conf.h"

uint8_t returnBuffer[VENDER_DATA_LENGTH];

uint8_t* CMD_Exec(uint8_t* cmd) {
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
          ActionConfig* dest = actionConfigs + id;
          uint8_t* src = &cmd[3];
          memcpy(dest, src, sizeof(ActionConfig));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // action read
          uint8_t id = cmd[2];
          ActionConfig* src = actionConfigs + id;
          strcpy(returnBuffer, "S\0");
          uint8_t* dest = &returnBuffer[2];
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
          RGB* dest = rgbIndex + id;
          uint8_t* src = &cmd[3];
          memcpy(dest, src, sizeof(uint16_t));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // lighting read
          uint8_t id = cmd[2];
          RGB* src = rgbIndex + id;
          strcpy(returnBuffer, "S\0");
          uint8_t* dest = &returnBuffer[2];
          memcpy(dest, src, sizeof(uint16_t));
        }
        break;
      default:
        strcpy(returnBuffer, "E: Unknown command for lighting");
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
          uint16_t* dest = lightMIDICCMappings + id;
          uint8_t* src = &cmd[3];
          memcpy(dest, src, sizeof(uint16_t));
          strcpy(returnBuffer, "S: OK");
        }
        break;
      case 'R':
        {
          // light-MIDICC mapping read
          uint8_t id = cmd[2];
          uint16_t* src = lightMIDICCMappings + id;
          strcpy(returnBuffer, "S\0");
          uint8_t* dest = &returnBuffer[2];
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
    }
    break;
  case 'R':
    {
      // reset
      FlashConfig_Reset();
    }
    break;
  default:
    strcpy(returnBuffer, "E: Unknown command");
    break;
  }

  return returnBuffer;
}
