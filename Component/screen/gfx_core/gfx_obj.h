#ifndef __GFX_OBJ_H__
#define __GFX_OBJ_H__

#include "gfx.h"
#include "gfx_ll.h"
#include "gfx_area.h"
#include "gfx_style.h"

struct _gfx_obj_t;

typedef bool (*gfx_design_func_t)(struct _gfx_obj_t* obj, gfx_area_t* mask_p);

typedef struct _gfx_obj_t {
    struct _gfx_obj_t* par; /* Pointer to the parent object */

    gfx_ll_t child_ll;      /* Linked list to store the children objects */

    gfx_style_t* style_p;   /* Pointer to the object's style */

    gfx_design_func_t design_func;  /* Object type specific design function */

    gfx_area_t coords;      /* Coordinates of the object (x1, y1, x2, y2) */

    gfx_coord_t ext_size;
} gfx_obj_t;


void gfx_init(void);
void gfx_obj_invalidate(gfx_obj_t* obj);
gfx_obj_t* gfx_scr_current(void);
gfx_obj_t* gfx_obj_create(gfx_obj_t* parent);
gfx_obj_t * gfx_obj_get_screen(const gfx_obj_t * obj);
gfx_obj_t * gfx_obj_get_parent(const gfx_obj_t * obj);
gfx_obj_t * gfx_obj_get_child(const gfx_obj_t * obj, const gfx_obj_t * child);
gfx_obj_t * gfx_obj_get_child_back(const gfx_obj_t * obj, const gfx_obj_t * child);
gfx_coord_t gfx_obj_get_width(const gfx_obj_t * obj);
gfx_coord_t gfx_obj_get_height(const gfx_obj_t * obj);

#endif /* __GFX_OBJ_H__ */

