#ifndef __GFX_VDB_H__
#define __GFX_VDB_H__

#include "gfx.h"
#include "gfx_area.h"
#include "gfx_color.h"

#ifndef GFX_VDB_PX_BPP
#define GFX_VDB_PX_BPP GFX_COLOR_SIZE     /* Default is GFX_COLOR_SIZE */
#endif /* GFX_VDB_PX_BPP */

#if GFX_VDB_SIZE == 0
#error "Invalid GFX_VDB_SIZE in gfx_conf.h!"
#endif /*GFX_VDB_SIZE */

/* The size of VDB in bytes.
 * (GFX_VDB_SIZE * GFX_VDB_PX_BPP) >> 3): just divide by 8 to convert bits to bytes
 * (((GFX_VDB_SIZE * GFX_VDB_PX_BPP) & 0x7) ? 1 : 0): add an extra byte to round up.
 *    E.g. if GFX_VDB_SIZE = 10 and GFX_VDB_PX_BPP = 1 -> 10 bits -> 2 bytes*/
#define GFX_VDB_SIZE_IN_BYTES ((GFX_VDB_SIZE * GFX_VDB_PX_BPP) >> 3) + (((GFX_VDB_SIZE * GFX_VDB_PX_BPP) & 0x7) ? 1 : 0)

typedef struct {
    gfx_area_t area;
    gfx_color_t* buf;
} gfx_vdb_t;

void gfx_vdb_flush(void);
gfx_vdb_t* gfx_vdb_get(void);

#endif /* __GFX_VDB_H__ */

