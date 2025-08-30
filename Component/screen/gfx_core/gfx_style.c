#include "gfx_style.h"
#include "gfx_obj.h"
#include "gfx_alloc.h"

gfx_style_t gfx_style_default;

gfx_style_t* gfx_style_create(void)
{
    gfx_style_t* new_style = gfx_mem_alloc(sizeof(gfx_style_t));

    return new_style;
}

void gfx_get_style_default(gfx_style_t* style_p)
{
    style_p->body.empty      = 0;
    style_p->body.main_color = GFX_COLOR_WHITE;

    style_p->border.color = GFX_COLOR_BLACK;
    style_p->border.width = 0;
    style_p->border.part  = GFX_BORDER_FULL;

    style_p->text.color   = GFX_COLOR_BLACK;
    style_p->text.font    = GFX_FONT_DEFAULT;
    style_p->text.letter_space = 2;
    style_p->text.line_space   = 2;
}

