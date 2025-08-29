#include "gfx_style.h"
#include "gfx_obj.h"

gfx_style_t gfx_style_default;

gfx_style_t* gfx_get_style_default(void)
{
    gfx_style_default.body.empty      = 0;
    gfx_style_default.body.main_color = GFX_COLOR_WHITE;

    gfx_style_default.border.color = GFX_COLOR_BLACK;
    gfx_style_default.border.width = 0;
    gfx_style_default.border.part  = GFX_BORDER_FULL;

    gfx_style_default.text.color   = GFX_COLOR_BLACK;
    gfx_style_default.text.font    = GFX_FONT_DEFAULT;
    gfx_style_default.text.letter_space = 2;
    gfx_style_default.text.line_space   = 2;

    return &gfx_style_default;
}

