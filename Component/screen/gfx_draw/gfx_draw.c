#include "gfx_draw.h"
#include "gfx_vdb.h"
#include "gfx_log.h"

static void gfx_buff_fill(gfx_area_t* mem_area, gfx_color_t* mem, const gfx_area_t* fill_area, gfx_color_t color);

/**
 * Fill an area in the Virtual Display Buffer
 * @param cords_p coordinates of the area to fill
 * @param mask_p fill only o this mask  (truncated to VDB area)
 * @param color fill color
 */
void gfx_draw_fill(const gfx_area_t* coords, const gfx_area_t * mask_p, gfx_color_t color)
{
    bool ret = false;
    gfx_area_t res_a;
    gfx_area_t vdp_rel_a;
    gfx_vdb_t* vdp_p = NULL;
    gfx_coord_t vdp_width = 0;
    gfx_color_t* vdp_buf_p = NULL;

    vdp_p = gfx_vdb_get();

    if (!vdp_p)
    {
        GFX_LOG_TRACE("Invalid VDB pointer");
        return;
    }

    ret = gfx_area_intersect(&res_a, &vdp_p->area, mask_p);
    if (ret == false) return;

    vdp_rel_a.x1 = res_a.x1 - vdp_p->area.x1;
    vdp_rel_a.y1 = res_a.y1 - vdp_p->area.y1;
    vdp_rel_a.x2 = res_a.x2 - vdp_p->area.x1;
    vdp_rel_a.y2 = res_a.y2 - vdp_p->area.y1;

    vdp_buf_p = vdp_p->buf;
    vdp_width = gfx_area_get_width(&vdp_p->area);
    vdp_buf_p = vdp_buf_p + vdp_width*vdp_rel_a.y1;

    gfx_buff_fill(&vdp_p->area, vdp_buf_p, &vdp_rel_a, color);
}

static void gfx_buff_fill(gfx_area_t* mem_area, gfx_color_t* mem, const gfx_area_t* fill_area, gfx_color_t color)
{
    gfx_coord_t col = 0;
    gfx_coord_t row = 0;
    gfx_coord_t vdp_width = gfx_area_get_width(mem_area);

    for (col = fill_area->x1; col <= fill_area->x2; ++col)
    {
        mem[col] = color;
    }

    gfx_color_t* mem_first = &mem[fill_area->x1];
    gfx_coord_t copy_size = (fill_area->x2 - fill_area->x1 + 1) * sizeof(gfx_color_t);
    mem = mem + vdp_width;

    for (row = fill_area->y1 + 1; row <= fill_area->y2; ++row)
    {
        memcpy(&mem[fill_area->x1], mem_first, copy_size);
        mem = mem + vdp_width;
    }
}

