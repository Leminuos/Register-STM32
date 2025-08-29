#ifndef __GFX_AREA_H__
#define __GFX_AREA_H__

#include "gfx.h"
#include "string.h"
#include "stdbool.h"

typedef int16_t gfx_coord_t;

typedef struct {
    gfx_coord_t x1;
    gfx_coord_t y1;
    gfx_coord_t x2;
    gfx_coord_t y2;
} gfx_area_t;

/**
 * Copy an area
 * @param dest pointer to the destination area
 * @param src pointer to the source area
 */
inline static void gfx_area_copy(gfx_area_t * dest, const gfx_area_t * src)
{
    memcpy(dest, src, sizeof(gfx_area_t));
}

/**
 * Get the width of an area
 * @param area_p pointer to an area
 * @return the width of the area (if x1 == x2 -> width = 1)
 */
static inline gfx_coord_t gfx_area_get_width(const gfx_area_t * area_p)
{
    return area_p->x2 - area_p->x1 + 1;
}

/**
 * Get the height of an area
 * @param area_p pointer to an area
 * @return the height of the area (if y1 == y2 -> height = 1)
 */
static inline gfx_coord_t gfx_area_get_height(const gfx_area_t * area_p)
{
    return area_p->y2 - area_p->y1 + 1;
}

/**
 * Set the width of an area
 * @param area_p pointer to an area
 * @param w the new width of the area
 */
static inline void gfx_area_set_width(gfx_area_t * area_p, gfx_coord_t w)
{
    area_p->x2 = area_p->x1 + w - 1;
}

/**
 * Set the height of an area
 * @param area_p pointer to an area
 * @param h the new height of the area
 */
static inline void gfx_area_set_height(gfx_area_t * area_p, gfx_coord_t h)
{
    area_p->y2 = area_p->y1 + h - 1;
}


uint32_t gfx_area_get_size(const gfx_area_t * area_p);
bool gfx_area_is_on(const gfx_area_t * a1_p, const gfx_area_t * a2_p);
bool gfx_area_is_in(const gfx_area_t * achild_p, const gfx_area_t * apar_t);
void gfx_area_set_pos(gfx_area_t * area_p, gfx_coord_t x, gfx_coord_t y);
void gfx_area_join(gfx_area_t * a_res_p, const gfx_area_t * a1_p, const gfx_area_t * a2_p);
bool gfx_area_intersect(gfx_area_t * res_p, const gfx_area_t * a1_p, const gfx_area_t * a2_p);
void gfx_area_set(gfx_area_t * area_p, gfx_coord_t x1, gfx_coord_t y1, gfx_coord_t x2, gfx_coord_t y2);

#endif /* __GFX_AREA_H__ */

