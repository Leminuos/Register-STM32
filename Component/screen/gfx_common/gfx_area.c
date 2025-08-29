#include "gfx_area.h"
#include "gfx_math.h"

/*
 * Area with the coordinates of the vertices
 *
 *      (x1, y2) +---------------+ (x2, y2)
 *               |               |
 *               |               |
 *      (x1, y1) +---------------+ (x2, y1)
 *
 * Where:
 *   - A(x1, y1): upper left corner
 *   - B(x2, y1): upper right corner
 *   - C(x2, y2): lower right corner
 *   - D(x1, y2): lower left corner
 */

/**
 * Initialize an area
 * @param area_p pointer to an area
 * @param x1 left coordinate of the area
 * @param y1 top coordinate of the area
 * @param x2 right coordinate of the area
 * @param y2 bottom coordinate of the area
 */
void gfx_area_set(gfx_area_t * area_p, gfx_coord_t x1, gfx_coord_t y1, gfx_coord_t x2, gfx_coord_t y2)
{
    area_p->x1 = x1;
    area_p->x2 = x2;
    area_p->y1 = y1;
    area_p->y2 = y2;
}

/**
 * Set the position of an area (width and height will be kept)
 * @param area_p pointer to an area
 * @param x the new x coordinate of the area
 * @param y the new y coordinate of the area
 */
void gfx_area_set_pos(gfx_area_t * area_p, gfx_coord_t x, gfx_coord_t y)
{
    gfx_coord_t w = gfx_area_get_width(area_p);
    gfx_coord_t h = gfx_area_get_height(area_p);

    area_p->x1 = x;
    area_p->y1 = y;

    gfx_area_set_width(area_p, w);
    gfx_area_set_height(area_p, h);
}

/**
 * Return with area of an area (x * y)
 * @param area_p pointer to an area
 * @return size of area
 */
uint32_t gfx_area_get_size(const gfx_area_t * area_p)
{
    uint32_t size;

    size = (uint32_t)(area_p->x2 - area_p->x1 + 1) *
           (area_p->y2 - area_p->y1 + 1);

    return size;
}

/**
 * Get the common parts of two areas
 * @param res_p pointer to an area, the result will be stored her
 * @param a1_p pointer to the first area
 * @param a2_p pointer to the second area
 * @return false: the two area has NO common parts, res_p is invalid
 */
bool gfx_area_intersect(gfx_area_t * res_p, const gfx_area_t * a1_p, const gfx_area_t * a2_p)
{
    bool union_ok = true;

    res_p->x1 = GFX_MATH_MAX(a1_p->x1, a1_p->x1);
    res_p->y1 = GFX_MATH_MAX(a1_p->y1, a1_p->y1);
    res_p->x2 = GFX_MATH_MIN(a1_p->x2, a1_p->x2);
    res_p->y2 = GFX_MATH_MIN(a1_p->y2, a1_p->y2);

    if ((res_p->x1 > res_p->x2) || (res_p->y1 > res_p->y2))
    {
        union_ok = false;
    }

    return union_ok;
}

/**
 * Join two areas into a third which involves the other two
 * @param res_p pointer to an area, the result will be stored here
 * @param a1_p pointer to the first area
 * @param a2_p pointer to the second area
 */
void gfx_area_join(gfx_area_t * a_res_p, const gfx_area_t * a1_p, const gfx_area_t * a2_p)
{
    a_res_p->x1 = GFX_MATH_MIN(a1_p->x1, a2_p->x1);
    a_res_p->y1 = GFX_MATH_MIN(a1_p->y1, a2_p->y1);
    a_res_p->x2 = GFX_MATH_MAX(a1_p->x2, a2_p->x2);
    a_res_p->y2 = GFX_MATH_MAX(a1_p->y2, a2_p->y2);
}

/**
 * Check if two area has common parts
 * @param a1_p pointer to an area.
 * @param a2_p pointer to an other area
 * @return false: a1_p and a2_p has no common parts
 */
bool gfx_area_is_on(const gfx_area_t * a1_p, const gfx_area_t * a2_p)
{
    bool is_on = false;
    
    if( (a1_p->x1 <= a2_p->x2) &&
        (a1_p->x2 >= a2_p->x1) &&
        (a1_p->y1 <= a2_p->y2) &&
        (a1_p->y2 >= a2_p->y1)
    )
    {
        is_on =  true;
    }

    return is_on;
}

/**
 * Check if an area is fully on an other
 * @param achild_p pointer to an child area
 * @param apar_t pointer to an parent area
 * @return
 */
bool gfx_area_is_in(const gfx_area_t * achild_p, const gfx_area_t * apar_t)
{
    bool is_in = false;

    if( (achild_p->x1  >= apar_t->x1) &&
        (achild_p->y1  >= apar_t->y1) &&
        (achild_p->x2  <= apar_t->x2) &&
        (achild_p->y2  <= apar_t->y2)
    )
    {
        is_in = true;
    }

    return is_in;
}

