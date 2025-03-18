#ifndef __OLED_FONTS_H__
#define __OLED_FONTS_H__

#include "oled.h"

#ifdef OLED_INCLUDE_FONT_6x8
extern const OLED_FontTypedef Font_6x8;
#endif
#ifdef OLED_INCLUDE_FONT_7x10
extern const OLED_FontTypedef Font_7x10;
#endif
#ifdef OLED_INCLUDE_FONT_11x18
extern const OLED_FontTypedef Font_11x18;
#endif
#ifdef OLED_INCLUDE_FONT_16x26
extern const OLED_FontTypedef Font_16x26;
#endif
#ifdef OLED_INCLUDE_FONT_16x24
extern const OLED_FontTypedef Font_16x24;
#endif
#ifdef OLED_INCLUDE_FONT_16x15
extern const OLED_FontTypedef Font_16x15;
#endif

#endif // __OLED_FONTS_H__
