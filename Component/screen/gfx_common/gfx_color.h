#ifndef __GFX_COLOR_H__
#define __GFX_COLOR_H__

#include "gfx.h"

#define GFX_COLOR_WHITE   GFX_COLOR_MAKE(0xFF,0xFF,0xFF)
#define GFX_COLOR_SILVER  GFX_COLOR_MAKE(0xC0,0xC0,0xC0)
#define GFX_COLOR_GRAY    GFX_COLOR_MAKE(0x80,0x80,0x80)
#define GFX_COLOR_BLACK   GFX_COLOR_MAKE(0x00,0x00,0x00)
#define GFX_COLOR_RED     GFX_COLOR_MAKE(0xFF,0x00,0x00)
#define GFX_COLOR_MAROON  GFX_COLOR_MAKE(0x80,0x00,0x00)
#define GFX_COLOR_YELLOW  GFX_COLOR_MAKE(0xFF,0xFF,0x00)
#define GFX_COLOR_OLIVE   GFX_COLOR_MAKE(0x80,0x80,0x00)
#define GFX_COLOR_LIME    GFX_COLOR_MAKE(0x00,0xFF,0x00)
#define GFX_COLOR_GREEN   GFX_COLOR_MAKE(0x00,0x80,0x00)
#define GFX_COLOR_CYAN    GFX_COLOR_MAKE(0x00,0xFF,0xFF)
#define GFX_COLOR_AQUA    GFX_COLOR_CYAN
#define GFX_COLOR_TEAL    GFX_COLOR_MAKE(0x00,0x80,0x80)
#define GFX_COLOR_BLUE    GFX_COLOR_MAKE(0x00,0x00,0xFF)
#define GFX_COLOR_NAVY    GFX_COLOR_MAKE(0x00,0x00,0x80)
#define GFX_COLOR_MAGENTA GFX_COLOR_MAKE(0xFF,0x00,0xFF)
#define GFX_COLOR_PURPLE  GFX_COLOR_MAKE(0x80,0x00,0x80)
#define GFX_COLOR_ORANGE  GFX_COLOR_MAKE(0xFF,0xA5,0x00)

#if GFX_COLOR_DEPTH == 1
#define GFX_COLOR_SIZE           8
#elif GFX_COLOR_DEPTH == 8
#define GFX_COLOR_SIZE           8
#elif GFX_COLOR_DEPTH == 16
#define GFX_COLOR_SIZE           16
#elif GFX_COLOR_DEPTH == 32
#define GFX_COLOR_SIZE           32
#else
#error "Invalid GFX_COLOR_DEPTH in gfx_conf.h! Set it to 1, 8, 16 or 32!"
#endif

typedef union
{
    uint8_t blue  :1;
    uint8_t green :1;
    uint8_t red   :1;
    uint8_t full  :1;
} gfx_color1_t;

typedef union
{
    struct
    {
        uint8_t blue  :2;
        uint8_t green :3;
        uint8_t red   :3;
    };
    uint8_t full;
} gfx_color8_t;

typedef union
{
    struct
    {
#if GFX_COLOR_16_SWAP == 0
        uint16_t blue  :5;
        uint16_t green :6;
        uint16_t red   :5;
#else
        uint16_t green_h :3;
        uint16_t red     :5;
        uint16_t blue    :5;
        uint16_t green_l :3;
#endif
    };
    uint16_t full;
} gfx_color16_t;

typedef union
{
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
    uint32_t full;
} gfx_color32_t;

#if GFX_COLOR_DEPTH == 1
typedef uint8_t gfx_color_int_t;
typedef gfx_color1_t gfx_color_t;
#elif GFX_COLOR_DEPTH == 8
typedef uint8_t gfx_color_int_t;
typedef gfx_color8_t gfx_color_t;
#elif GFX_COLOR_DEPTH == 16
typedef uint16_t gfx_color_int_t;
typedef gfx_color16_t gfx_color_t;
#elif GFX_COLOR_DEPTH == 32
typedef uint32_t gfx_color_int_t;
typedef gfx_color32_t gfx_color_t;
#else
#error "Invalid GFX_COLOR_DEPTH in gfx_conf.h! Set it to 1, 8, 16 or 32!"
#endif

/* The most simple macro to create a color from R,G and B values
 * The order of bit field is different on Big-endian and Little-endian machines*/
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#if GFX_COLOR_DEPTH == 1
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){(b8 >> 7 | g8 >> 7 | r8 >> 7)})
#elif GFX_COLOR_DEPTH == 8
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{b8 >> 6, g8 >> 5, r8 >> 5}})
#elif GFX_COLOR_DEPTH == 16
#  if GFX_COLOR_16_SWAP == 0
#    define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{b8 >> 3, g8 >> 2, r8 >> 3}})
#  else
#    define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{g8 >> 5, r8 >> 3, b8 >> 3, (g8 >> 2) & 0x7}})
#  endif
#elif GFX_COLOR_DEPTH == 32
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{b8, g8, r8, 0xff}})            /*Fix 0xff alpha*/
#endif
#else
#if GFX_COLOR_DEPTH == 1
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){(r8 >> 7 | g8 >> 7 | b8 >> 7)})
#elif GFX_COLOR_DEPTH == 8
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{r8 >> 6, g8 >> 5, b8 >> 5}})
#elif GFX_COLOR_DEPTH == 16
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{r8 >> 3, g8 >> 2, b8 >> 3}})
#elif GFX_COLOR_DEPTH == 32
#define GFX_COLOR_MAKE(r8, g8, b8) ((gfx_color_t){{0xff, r8, g8, b8}})            /*Fix 0xff alpha*/
#endif
#endif


#define GFX_COLOR_HEX(c) GFX_COLOR_MAKE(((uint32_t)((uint32_t)c >> 16) & 0xFF), \
                                ((uint32_t)((uint32_t)c >> 8) & 0xFF), \
                                ((uint32_t) c & 0xFF))

/*Usage GFX_COLOR_HEX3(0x16C) which means GFX_COLOR_HEX(0x1166CC)*/
#define GFX_COLOR_HEX3(c) GFX_COLOR_MAKE((((c >> 4) & 0xF0) | ((c >> 8) & 0xF)),   \
                                ((uint32_t)(c & 0xF0)       | ((c & 0xF0) >> 4)), \
                                ((uint32_t)(c & 0xF)         | ((c & 0xF) << 4)))

static inline gfx_color_t gfx_color_hex(uint32_t c)
{
    return GFX_COLOR_HEX(c);
}

static inline gfx_color_t gfx_color_hex3(uint32_t c)
{
    return GFX_COLOR_HEX3(c);
}

#endif /* __GFX_COLOR_H__ */

