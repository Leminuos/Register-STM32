#include "gfx_obj.h"
#include "gfx_ll.h"
#include "gfx_log.h"
#include "gfx_task.h"
#include "gfx_alloc.h"
#include "gfx_color.h"
#include "gfx_refr.h"
#include "gfx_draw_rect.h"

#define GFX_OBJ_DEF_WIDTH   (GFX_DPI)
#define GFX_OBJ_DEF_HEIGHT  (2 * GFX_DPI / 3)

static gfx_ll_t  gfx_ll_scr;
static gfx_obj_t gfx_act_scr;

static void gfx_signal_screen(void);
static void gfx_design_screen(struct _gfx_obj_t* obj, gfx_area_t* mask_p);

void gfx_init(void)
{
    GFX_LOG_TRACE("gfx_init started");

    gfx_mem_init();
    gfx_task_init();
    gfx_refr_init();
    gfx_create_screen(GFX_VER_RES, GFX_HOR_RES);
    
    GFX_LOG_TRACE("gfx_init ready");
}

void gfx_create_screen(gfx_coord_t h, gfx_coord_t w)
{
    GFX_LOG_TRACE("Screen create started");

    /*Set coordinates to full screen size*/
    gfx_act_scr.coords.x1 = 0;
    gfx_act_scr.coords.y1 = 0;
    gfx_act_scr.coords.x2 = w - 1;
    gfx_act_scr.coords.y2 = h - 1;
    gfx_act_scr.ext_size  = 0;

    gfx_act_scr.style_p = gfx_get_style_default();
    gfx_act_scr.signal_func = gfx_signal_screen;
    gfx_act_scr.design_func = gfx_design_screen;

    gfx_ll_init(&gfx_ll_scr, sizeof(gfx_obj_t));

    GFX_LOG_TRACE("Screen create ready");
}

static void gfx_design_screen(struct _gfx_obj_t* obj, gfx_area_t* mask_p)
{
    gfx_draw_rect(mask_p, obj->style_p);
}

static void gfx_signal_screen(void)
{

}

gfx_obj_t* gfx_obj_create(void)
{
    GFX_LOG_TRACE("Object create started");
    gfx_obj_t* new_obj = NULL;
    new_obj = gfx_ll_ins_head(&gfx_ll_scr);
    if (new_obj == NULL) return NULL;

    new_obj->coords.x1 = gfx_act_scr.coords.x1;
    new_obj->coords.y1 = gfx_act_scr.coords.y1;
    new_obj->coords.x2 = gfx_act_scr.coords.x1 + GFX_OBJ_DEF_WIDTH - 1;
    new_obj->coords.y2 = gfx_act_scr.coords.y1 + GFX_OBJ_DEF_HEIGHT - 1;
    new_obj->ext_size  = 0;

    new_obj->signal_func = NULL;
    new_obj->design_func = NULL;
    new_obj->style_p = gfx_get_style_default();

    GFX_LOG_TRACE("Object create successfully");

    return new_obj;
}

void gfx_obj_invalidate(gfx_obj_t* obj)
{
    gfx_obj_t* obj_scr = gfx_obj_get_screen();

    /*Truncate recursively to the parents*/
    bool ret = false;
    gfx_area_t area_trunc;

    /*Start with the original coordinates*/
    gfx_coord_t ext_size = obj->ext_size;
    gfx_area_copy(&area_trunc, &obj->coords);
    area_trunc.x1 = area_trunc.x1 + ext_size;
    area_trunc.y1 = area_trunc.y1 + ext_size;
    area_trunc.x2 = area_trunc.x2 - ext_size;
    area_trunc.y2 = area_trunc.y2 - ext_size;

    ret = gfx_area_intersect(&area_trunc, &area_trunc, &obj_scr->coords);
    if (ret != false) gfx_inv_area(&area_trunc);
}

/**
 * Return pointer to a screen
 * @return pointer to a screen
 */
gfx_obj_t* gfx_obj_get_screen(void)
{
    return &gfx_act_scr;
}

gfx_ll_t* gfx_obj_ll_screen(void)
{
    return &gfx_ll_scr;
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
