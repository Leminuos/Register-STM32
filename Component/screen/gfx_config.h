#ifndef __GFX_CONF_H__
#define __GFX_CONF_H__

/* Horizontal and vertical resolution of the library.*/
#define GFX_HOR_RES                     (320U)
#define GFX_VER_RES                     (280U)

/* Dot Per Inch: used to initialize default sizes. E.g. a button with width = GFX_DPI / 2 -> half inch wide
 * (Not so important, you can adjust it to modify default sizes and spaces)*/
#define GFX_DPI                         100

/*Screen refresh period in milliseconds*/
#define GFX_REFR_PERIOD                 30

/* Size of the VDB in pixels. Typical size: ~1/10 screen. Must be >= LV_HOR_RES
 * Setting it to 0 will disable VDB */
#define GFX_VDB_SIZE                    ((GFX_HOR_RES * GFX_VER_RES) / 32U)

/* Enable anti-aliasing (lines, and radiuses will be smoothed) */
#define LV_ANTIALIAS                    1       /*1: Enable anti-aliasing*/

#define TASK_PRIO_MAX_SIZE              8

#define USE_MEM_CUSTOM                  1
#if USE_MEM_CUSTOM
#include "mem.h"
#define GFX_MEM_INIT_CUSTOM()           mem_init()
#define GFX_MALLOC_CUSTOM(_size)        malloc(_size)
#define GFX_MALLOC_FREE(_block)         free(_block)
#endif /* USE_MEM_CUSTOM */

#define USE_TICK_CUSTOM                 0
#if USE_TICK_CUSTOM
#define GFX_TICK_CUSTOM_SYS_TIME_EXPR   0
#endif /* USE_TICK_CUSTOM */

#define GFX_COLOR_DEPTH                 16
#if GFX_COLOR_DEPTH == 16
#define GFX_COLOR_16_SWAP               0
#endif /* GFX_COLOR_DEPTH */

#endif /* __GFX_CONF_H__ */
