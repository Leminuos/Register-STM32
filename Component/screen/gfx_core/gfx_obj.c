#include "gfx_obj.h"
#include "gfx_ll.h"
#include "gfx_log.h"
#include "gfx_task.h"
#include "gfx_alloc.h"
#include "gfx_color.h"
#include "gfx_refr.h"

#define GFX_OBJ_DEF_WIDTH   (GFX_DPI)
#define GFX_OBJ_DEF_HEIGHT  (2 * GFX_DPI / 3)

static gfx_ll_t  gfx_ll_scr;
static gfx_obj_t* gfx_cur_scr;
static gfx_obj_t* gfx_tmp_scr;

void gfx_init(void)
{
    GFX_LOG_TRACE("gfx_init started");

    gfx_mem_init();
    gfx_task_init();
    gfx_refr_init();

    gfx_ll_init(&gfx_ll_scr, sizeof(gfx_obj_t));
    gfx_tmp_scr = gfx_obj_create(NULL);
    gfx_cur_scr = gfx_tmp_scr;

    GFX_LOG_TRACE("gfx_init ready");
}

gfx_obj_t* gfx_obj_create(gfx_obj_t* parent)
{
    gfx_obj_t* new_obj = NULL;

    if (parent == NULL)
    {
        GFX_LOG_TRACE("Screen create started");
        new_obj = gfx_ll_ins_head(&gfx_ll_scr);
        if (new_obj == NULL) return NULL;

        new_obj->par = NULL;
        gfx_ll_init(&new_obj->child_ll, sizeof(gfx_obj_t));

        /*Set coordinates to full screen size*/
        new_obj->coords.x1 = 0;
        new_obj->coords.y1 = 0;
        new_obj->coords.x2 = GFX_HOR_RES - 1;
        new_obj->coords.y2 = GFX_VER_RES - 1;
        new_obj->ext_size  = 0;

        GFX_LOG_TRACE("Screen create ready");
    }
    else
    {
        GFX_LOG_TRACE("Object create started");
        new_obj = gfx_ll_ins_head(&parent->child_ll);
        if (new_obj == NULL) return NULL;

        new_obj->par = parent;
        gfx_ll_init(&new_obj->child_ll, sizeof(gfx_obj_t));

        new_obj->coords.x1 = parent->coords.x1;
        new_obj->coords.y1 = parent->coords.y1;
        new_obj->coords.x2 = new_obj->coords.x1 + GFX_OBJ_DEF_WIDTH - 1;
        new_obj->coords.y2 = new_obj->coords.y2 + GFX_OBJ_DEF_HEIGHT - 1;
        new_obj->ext_size  = 0;
    }

    if (parent != NULL)
    {
        /*Invalidate the area if not screen created*/
        gfx_obj_invalidate(new_obj);
    }

    return new_obj;
}

void gfx_obj_invalidate(gfx_obj_t* obj)
{
    gfx_obj_t* obj_scr = gfx_obj_get_screen(obj);

    if (obj_scr == gfx_cur_scr)
    {
        /*Truncate recursively to the parents*/
        bool ret = true;
        gfx_area_t area_trunc;
        gfx_obj_t* par = gfx_obj_get_parent(obj);

        /*Start with the original coordinates*/
        gfx_coord_t ext_size = obj->ext_size;
        gfx_area_copy(&area_trunc, &obj->coords);
        area_trunc.x1 = area_trunc.x1 - ext_size;
        area_trunc.y1 = area_trunc.y1 - ext_size;
        area_trunc.x2 = area_trunc.x2 + ext_size;
        area_trunc.y2 = area_trunc.y2 + ext_size;

        while (par != NULL)
        {
            ret = gfx_area_intersect(&area_trunc, &area_trunc, &par->coords);
            if (ret == false) break;
            par = gfx_obj_get_parent(par);
        }

        if (ret != false) gfx_inv_area(&area_trunc);
    }
}

/**
 * Return with the screen of an object
 * @param obj pointer to an object
 * @return pointer to a screen
 */
gfx_obj_t * gfx_obj_get_screen(const gfx_obj_t * obj)
{
    const gfx_obj_t * par = obj;
    const gfx_obj_t * act_p;

    do {
        act_p = par;
        par = gfx_obj_get_parent(act_p);
    } while(par != NULL);

    return (gfx_obj_t *)act_p;
}

/**
 * Returns with the parent of an object
 * @param obj pointer to an object
 * @return pointer to the parent of  'obj'
 */
gfx_obj_t * gfx_obj_get_parent(const gfx_obj_t * obj)
{
    return obj->par;
}

/**
 * Iterate through the children of an object (start from the "youngest")
 * @param obj pointer to an object
 * @param child NULL at first call to get the next children
 *                  and the previous return value later
 * @return the child after 'act_child' or NULL if no more child
 */
gfx_obj_t * gfx_obj_get_child(const gfx_obj_t * obj, const gfx_obj_t * child)
{
    gfx_obj_t * result = NULL;

    if(child == NULL) {
        result = gfx_ll_get_head(&obj->child_ll);
    } else {
        result = gfx_ll_get_next(&obj->child_ll, child);
    }

    return result;
}

/**
 * Iterate through the children of an object (start from the "oldest")
 * @param obj pointer to an object
 * @param child NULL at first call to get the next children
 *                  and the previous return value later
 * @return the child after 'act_child' or NULL if no more child
 */
gfx_obj_t * gfx_obj_get_child_back(const gfx_obj_t * obj, const gfx_obj_t * child)
{
    gfx_obj_t * result = NULL;

    if(child == NULL) {
        result = gfx_ll_get_tail(&obj->child_ll);
    } else {
        result = gfx_ll_get_prev(&obj->child_ll, child);
    }

    return result;
}

/**
 * Get the width of an object
 * @param obj pointer to an object
 * @return the width
 */
gfx_coord_t gfx_obj_get_width(const gfx_obj_t * obj)
{
    return gfx_area_get_width(&obj->coords);
}

/**
 * Get the height of an object
 * @param obj pointer to an object
 * @return the height
 */
gfx_coord_t gfx_obj_get_height(const gfx_obj_t * obj)
{
    return gfx_area_get_height(&obj->coords);
}

gfx_obj_t* gfx_scr_current(void)
{
    return gfx_cur_scr;
}
