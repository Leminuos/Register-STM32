#ifndef __GFX_BUTTON_H__
#define __GFX_BUTTON_H__

#include "gfx.h"
#include "gfx_obj.h"

gfx_obj_t* gfx_create_button(gfx_coord_t w, gfx_coord_t h);
void gfx_btn_set_border(gfx_obj_t* obj, gfx_coord_t width,  gfx_border_part_t path);

#endif /* __GFX_BUTTON_H__ */

