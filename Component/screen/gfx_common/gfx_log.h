#ifndef __GFX_LOG_H__
#define __GFX_LOG_H__

#include "gfx.h"
#include "debug.h"

#define GFX_LOG_TRACE(format,...)   DEBUG(LOG_VERBOSE, "lvgl", format, ##__VA_ARGS__)

#endif /* __GFX_LOG_H__ */

