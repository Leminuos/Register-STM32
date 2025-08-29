#ifndef __GFX_STYLE_H__
#define __GFX_STYLE_H__

#include "gfx.h"
#include "gfx_color.h"
#include "gfx_area.h"

enum
{
    GFX_BORDER_NONE =     0x00,
    GFX_BORDER_BOTTOM =   0x01,
    GFX_BORDER_TOP =      0x02,
    GFX_BORDER_LEFT =     0x04,
    GFX_BORDER_RIGHT =    0x08,
    GFX_BORDER_FULL =     0x0F,
    GFX_BORDER_INTERNAL = 0x10,    /*FOR matrix-like objects (e.g. Button matrix)*/
};
typedef uint8_t gfx_border_part_t;

typedef struct {
    const uint8_t width;
    const uint8_t height;
} gfx_font_t;

typedef struct {
    struct {
        gfx_color_t main_color;
        uint8_t     empty;
    } body;

    struct {
        gfx_color_t color;
        gfx_coord_t width;
        gfx_border_part_t part;
    } border;

    struct {
        gfx_color_t color;
        const gfx_font_t* font;
        gfx_coord_t letter_space;
        gfx_coord_t line_space;
    } text;
} gfx_style_t;

gfx_style_t* gfx_get_style_default(void);

#endif /* __GFX_STYLE_H__ */

