#include "gfx_vdb.h"
#include "gfx_log.h"

static uint8_t vdb_buf[GFX_VDB_SIZE_IN_BYTES];
static gfx_vdb_t vdb = {.buf = (gfx_color_t *)vdb_buf};
static volatile bool vdb_flushing = false;

gfx_vdb_t* gfx_vdb_get(void)
{
    // Wait until VDB is flushing
    while(vdb_flushing);

    return &vdb;
}

/**
 * Flush the content of the VDB
 */
void gfx_vdb_flush(void)
{
    gfx_vdb_t * vdb_curr = gfx_vdb_get();
    if(!vdb_curr) {
        GFX_LOG_TRACE("Invalid VDB pointer");
        return;
    }

    vdb_flushing = true;

    /*Flush the rendered content to the display*/
    #if 0
    gfx_disp_flush(vdb_curr->area.x1, vdb_curr->area.y1, vdb_curr->area.x2, vdb_curr->area.y2, vdb_curr->buf);
    #endif /* nguyen.bui need review */
}

