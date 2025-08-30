#include "gfx_draw_rect.h"
#include "gfx_log.h"

extern bool parent_check;

static void gfx_draw_rect_main_mid(const gfx_area_t* area_p, const gfx_area_t* mask, const gfx_style_t* style);
static void gfx_draw_rect_border_straight(const gfx_area_t* area_p, const gfx_area_t* mask, const gfx_style_t* style);

/**
 * Draw a rectangle
 * @param coords the coordinates of the original rectangle
 * @param mask the rectangle will be drawn only in this mask
 * @param style pointer to a style
 */
void gfx_draw_rect(const gfx_area_t* coords, const gfx_area_t* mask, const gfx_style_t* style)
{
    if(gfx_area_get_height(mask) < 1 || gfx_area_get_width(mask) < 1) return;

    if(style->body.empty == 0)
        gfx_draw_rect_main_mid(coords, mask, style);

    if(style->border.width != 0 && style->border.part != GFX_BORDER_NONE)
        gfx_draw_rect_border_straight(coords, mask, style);
}

/**
 * Draw the middle part (rectangular) of a rectangle
 * @param coords the coordinates of the original rectangle
 * @param mask the rectangle will be drawn only  on this area
 * @param rects_p pointer to a rectangle style
 */
static void gfx_draw_rect_main_mid(const gfx_area_t* coords, const gfx_area_t* mask, const gfx_style_t* style)
{
    gfx_color_t mcolor = style->body.main_color;
    gfx_draw_fill(coords, mask, mcolor);
}

/**
 * Draw the straight parts of a rectangle border
 * @param coords the coordinates of the original rectangle
 * @param mask_ the rectangle will be drawn only  on this area
 * @param rstyle pointer to a rectangle style
 */
static void gfx_draw_rect_border_straight(const gfx_area_t* coords, const gfx_area_t* mask, const gfx_style_t* style)
{
    uint16_t bwidth = style->border.width;
    gfx_border_part_t part = style->border.part;
    gfx_color_t color = style->border.color;
    gfx_area_t work_area;

    /*the 0 px border width drawn as 1 px, so decrement the b_width*/
    bwidth--;

    if(part & GFX_BORDER_TOP)
    {
        work_area.x1 = coords->x1;
        work_area.x2 = coords->x2;
        work_area.y1 = coords->y1;
        work_area.y2 = coords->y1 + bwidth;
        gfx_draw_fill(coords, &work_area, color);
    }

    if(part & GFX_BORDER_RIGHT)
    {
        work_area.x1 = coords->x2 - bwidth;
        work_area.x2 = coords->x2;
        work_area.y1 = coords->y1 + (part & GFX_BORDER_TOP ? bwidth + 1 : 0);
        work_area.y2 = coords->y2 - (part & GFX_BORDER_BOTTOM ? bwidth + 1 : 0);
        gfx_draw_fill(coords, &work_area, color);
    }

    if(part & GFX_BORDER_LEFT)
    {
        work_area.x1 = coords->x1;
        work_area.x2 = coords->x1 + bwidth;
        work_area.y1 = coords->y1 + (part & GFX_BORDER_TOP ? bwidth + 1 : 0);
        work_area.y2 = coords->y2 - (part & GFX_BORDER_BOTTOM ? bwidth + 1 : 0);
        gfx_draw_fill(coords, &work_area, color);
    }

    if(part & GFX_BORDER_BOTTOM)
    {
        work_area.x1 = coords->x1;
        work_area.x2 = coords->x2;
        work_area.y1 = coords->y2 - bwidth;
        work_area.y2 = coords->y2;
        gfx_draw_fill(coords, &work_area, color);
    }
}
