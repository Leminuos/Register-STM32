#ifndef __GFX_DRAW_RECT_H__
#define __GFX_DRAW_RECT_H__

#include "gfx_draw.h"

/**
 * Draw a rectangle
 * @param coords the coordinates of the rectangle
 * @param mask the rectangle will be drawn only in this mask
 * @param style pointer to a style
 */
void gfx_draw_rect(const gfx_area_t * coords, const gfx_area_t * mask, const gfx_style_t * style);

#endif /* __GFX_DRAW_RECT_H__ */

