#ifndef __FONTS__
#define __FONTS__

#include <stdint.h>

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *const data;
    const uint8_t *const char_width;
} FontTypedef;

#define INCLUDE_FONT_11x18
#define INCLUDE_FONT_16x15
#define SUPPORT_VIE

#ifdef INCLUDE_FONT_6x8
extern const FontTypedef Font_6x8;
#endif
#ifdef INCLUDE_FONT_7x10
extern const FontTypedef Font_7x10;
#endif
#ifdef INCLUDE_FONT_11x18
extern const FontTypedef Font_11x18;
#endif
#ifdef INCLUDE_FONT_16x26
extern const FontTypedef Font_16x26;
#endif
#ifdef INCLUDE_FONT_16x24
extern const FontTypedef Font_16x24;
#endif
#ifdef INCLUDE_FONT_16x15
extern const FontTypedef Font_16x15;
#endif

#endif // __FONTS__
