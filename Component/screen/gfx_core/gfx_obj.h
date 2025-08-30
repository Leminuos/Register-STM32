#ifndef __GFX_OBJ_H__
#define __GFX_OBJ_H__

#include "gfx.h"
#include "gfx_ll.h"
#include "gfx_area.h"
#include "gfx_style.h"

struct _gfx_obj_t;

typedef void (*gfx_design_func_t)(struct _gfx_obj_t* obj, gfx_area_t* mask_p);
typedef void (*gfx_signal_func_t)(void);

typedef struct _gfx_obj_t {
    gfx_style_t* style_p;   /* Pointer to the object's style */
    gfx_signal_func_t signal_func;
    gfx_design_func_t design_func;  /* Object type specific design function */
    gfx_area_t coords;      /* Coordinates of the object (x1, y1, x2, y2) */
    gfx_coord_t ext_size;
} gfx_obj_t;

void gfx_init(void);
void gfx_obj_invalidate(gfx_obj_t* obj);
void gfx_create_screen(gfx_coord_t h, gfx_coord_t w);
gfx_obj_t* gfx_obj_create(void);
gfx_ll_t* gfx_obj_ll_screen(void);
gfx_obj_t * gfx_obj_get_screen(void);
gfx_coord_t gfx_obj_get_width(const gfx_obj_t * obj);
gfx_coord_t gfx_obj_get_height(const gfx_obj_t * obj);
void gfx_obj_set_pos(gfx_obj_t * obj, gfx_coord_t x, gfx_coord_t y);

#endif /* __GFX_OBJ_H__ */

