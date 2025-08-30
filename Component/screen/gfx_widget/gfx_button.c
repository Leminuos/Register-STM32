#include "gfx_button.h"
#include "gfx_draw_rect.h"

static void gfx_signal_button(void);
static void gfx_design_button(gfx_obj_t* obj, gfx_area_t* mask_p);

gfx_obj_t* gfx_create_button(gfx_coord_t w, gfx_coord_t h)
{
    gfx_obj_t* new_obj = gfx_obj_create();

    new_obj->coords.x2 = new_obj->coords.x1 + w - 1;
    new_obj->coords.y2 = new_obj->coords.y1 + h - 1;

    new_obj->signal_func = gfx_signal_button;
    new_obj->design_func = gfx_design_button;

    gfx_obj_invalidate(new_obj);

    return new_obj;
}

void gfx_btn_set_border(gfx_obj_t* obj, gfx_coord_t width,  gfx_border_part_t path)
{
    obj->style_p->border.width = width;
    obj->style_p->border.part = path;

    gfx_obj_invalidate(obj);
}

static void gfx_design_button(gfx_obj_t* obj, gfx_area_t* mask_p)
{
    gfx_draw_rect(&obj->coords, mask_p, obj->style_p);
}

static void gfx_signal_button(void)
{

}

