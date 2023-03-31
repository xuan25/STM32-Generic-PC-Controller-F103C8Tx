#ifndef __CONFIG_RGB_H
#define __CONFIG_RGB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ws2812b.h"

extern RGB rgbIndex[];

#define RGB_RELEASED    (rgbIndex + 0)
#define RGB_PRESSED     (rgbIndex + 1)
#define RGB_TICKED_CW   (rgbIndex + 2)
#define RGB_TICKED_CCW  (rgbIndex + 3)
#define RGB_CLICKED     (rgbIndex + 4)
#define RGB_1_TEMP      (rgbIndex + 5)
#define RGB_2_A         (rgbIndex + 6)
#define RGB_3_A         (rgbIndex + 7)

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_RGB_H */
