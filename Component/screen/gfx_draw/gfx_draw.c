#include "gfx_draw.h"
#include "gfx_vdb.h"
#include "gfx_log.h"

static void gfx_buff_fill(gfx_color_t* mem, const gfx_area_t* fill_area, gfx_color_t color);

/**
 * Fill an area in the Virtual Display Buffer
 * @param mask_p fill only o this mask  (truncated to VDB area)
 * @param color fill color
 */
void gfx_draw_fill(const gfx_area_t * mask_p, gfx_color_t color)
{
    bool ret = false;
    gfx_area_t res_a;
    gfx_vdb_t* vdb_p = gfx_vdb_get();
    if (!vdb_p)
    {
        GFX_LOG_TRACE("Invalid VDB pointer");
        return;
    }

    ret = gfx_area_intersect(&res_a, &vdb_p->area, mask_p);

    if (ret == false) return;

    gfx_buff_fill(vdb_p->buf, &res_a, color);
}

static void gfx_buff_fill(gfx_color_t* mem, const gfx_area_t* fill_area, gfx_color_t color)
{
    gfx_coord_t col = 0;
    gfx_coord_t row = 0;
    gfx_color_int_t pixel = 0;

    for (row = fill_area->y1; row <= fill_area->y2; ++row)
    {
        for (col = fill_area->x1; col <= fill_area->x2; ++col)
        {
            mem[pixel] = color;
            pixel++;
        }
    }
}

