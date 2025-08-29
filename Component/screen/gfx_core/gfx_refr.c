#include "gfx_refr.h"
#include "gfx_task.h"
#include "gfx_log.h"
#include "gfx_vdb.h"
#include "gfx_obj.h"

#ifndef GFX_INV_FIFO_SIZE
#define GFX_INV_FIFO_SIZE       32U
#endif /* GFX_INV_FIFO_SIZE */

typedef struct {
    gfx_area_t area;
    uint8_t joined;
} gfx_join_t;

static gfx_join_t inv_buf[GFX_INV_FIFO_SIZE];
static uint16_t inv_buf_p;

static void gfx_refr_areas(void);
static void gfx_refr_join_area(void);
static void gfx_refr_task(void* param);
static void gfx_refr_obj(gfx_area_t* mask_p);
static void gfx_refr_area_vdb(gfx_area_t* area_p);
static void gfx_refr_area_partial_vdb(const gfx_area_t* area_p);

/**
 * Initialize the screen refresh subsystem
 */
void gfx_refr_init(void)
{
    inv_buf_p = 0;
    memset(inv_buf, 0, sizeof(inv_buf));

    gfx_task_t* task;
    task = gfx_task_create("Refresh tash", gfx_refr_task, GFX_REFR_PERIOD, GFX_TASK_PRIO_MID, NULL);
    gfx_task_ready(task);
}

static void gfx_refr_task(void* param)
{
    (void) param;

    gfx_refr_join_area();

    gfx_refr_areas();

    if (inv_buf_p != 0)
    {
        // Clean up
        inv_buf_p = 0;
        memset(inv_buf, 0, sizeof(inv_buf));
    }
}

/**
 * Invalidate an area
 * @param area_p pointer to area which should be invalidated
 */
void gfx_inv_area(const gfx_area_t * area_p)
{
    if (area_p == NULL)
    {
        inv_buf_p = 0;
        return;
    }

    bool ret = false;
    gfx_area_t tmp_area;
    gfx_obj_t* obj_scr = gfx_obj_get_screen();

    ret = gfx_area_intersect(&tmp_area, area_p, &obj_scr->coords);

    if (ret != false)
    {
        uint16_t i = 0;

        for (i = 0; i < inv_buf_p; ++i)
            if (gfx_area_is_in(&tmp_area, &inv_buf[i].area) != false)
                return;

        if (inv_buf_p < GFX_INV_FIFO_SIZE)
        {
            gfx_area_copy(&inv_buf[inv_buf_p].area, &tmp_area);
        }
        else
        {
            inv_buf_p = 0;
            gfx_area_copy(&inv_buf[inv_buf_p].area, &tmp_area);
        }

        inv_buf_p++;
    }
}

/**
 * Join the areas which has got common parts
 */
static void gfx_refr_join_area(void)
{

}

/**
 * Refresh the joined areas
 */
static void gfx_refr_areas(void)
{
    uint16_t i = 0;

    for (i = 0; i < inv_buf_p; ++i)
    {
        if (inv_buf[i].joined == 0)
        {
            gfx_refr_area_vdb(&inv_buf[i].area);
        }
    }
}

/**
 * Refresh an area if there is Virtual Display Buffer
 * @param area_p  pointer to an area to refresh
 */
static void gfx_refr_area_vdb(gfx_area_t* area_p)
{
    gfx_vdb_t* vdb_p = NULL;

    /*Calculate the max row num*/
    gfx_coord_t w = gfx_area_get_width(area_p);
    gfx_coord_t h = gfx_area_get_height(area_p);
    gfx_coord_t y2 = area_p->y2 > GFX_VER_RES ? (GFX_VER_RES - 1) : area_p->y2;

    uint32_t row = 0;
    uint32_t row_max = GFX_VDB_SIZE / w;
    uint32_t row_last = 0;

    if (row_max > h) h = row_max;

    for (row = area_p->y1; row + row_max - 1 <= y2; row = row + row_max)
    {
        vdb_p = gfx_vdb_get();
        if(!vdb_p)
        {
            GFX_LOG_TRACE("Invalid VDB pointer");
            return;
        }

        vdb_p->area.x1 = area_p->x1;
        vdb_p->area.x2 = area_p->x2;
        vdb_p->area.y1 = row;
        vdb_p->area.y2 = row + row_max - 1;
        if (vdb_p->area.y2 > y2) y2 = vdb_p->area.y2;
        row_last = vdb_p->area.y2;
        gfx_refr_area_partial_vdb(area_p);
    }

    if (row_last != y2)
    {
        vdb_p = gfx_vdb_get();
        if(!vdb_p)
        {
            GFX_LOG_TRACE("Invalid VDB pointer");
            return;
        }

        vdb_p->area.x1 = area_p->x1;
        vdb_p->area.x2 = area_p->x2;
        vdb_p->area.y1 = row;
        vdb_p->area.y2 = y2;

        gfx_refr_area_partial_vdb(area_p);
    }
}

/**
 * Refresh a part of an area which is on the actual Virtual Display Buffer
 * @param area_p pointer to an area to refresh
 */
static void gfx_refr_area_partial_vdb(const gfx_area_t* area_p)
{
    gfx_vdb_t* vdb_p = gfx_vdb_get();
    if(!vdb_p)
    {
        GFX_LOG_TRACE("Invalid VDB pointer");
        return;
    }

    gfx_area_t mask_area;
    gfx_area_intersect(&mask_area, area_p, &vdb_p->area);

    gfx_refr_obj(&mask_area);

    gfx_vdb_flush();
}

static void gfx_refr_obj(gfx_area_t* mask_p)
{
    bool ret = false;
    gfx_obj_t* obj_p = NULL;
    gfx_ll_t* ll_scr_p = gfx_obj_ll_screen();
    gfx_ll_node_t* next = NULL;

    for (next = ll_scr_p->head; next != NULL; next = gfx_ll_get_next(ll_scr_p, next))
    {
        obj_p = (gfx_obj_t*)next;

        ret = gfx_area_is_in(mask_p, &obj_p->coords);
        if (ret == true)
        {
            obj_p->design_func(obj_p, mask_p);
        }
    }

    if (ret == false)
    {
        obj_p = gfx_obj_get_screen();
        obj_p->design_func(obj_p, mask_p);
    }
}
