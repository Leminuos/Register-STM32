#include <string.h>
#include "gfx_alloc.h"

void gfx_mem_init(void)
{
#if USE_MEM_CUSTOM == 0
#else
    GFX_MEM_INIT_CUSTOM();
#endif /* USE_MEM_CUSTOM */
}

void* gfx_mem_alloc(uint32_t size)
{
    void* alloc = NULL;

#if USE_MEM_CUSTOM == 0
#else
    alloc = GFX_MALLOC_CUSTOM(size);
    memset(alloc, 0, size);
#endif /* USE_MEM_CUSTOM */

    return alloc;
}

void gfx_mem_free(void* data)
{
#if USE_MEM_CUSTOM == 0
#else
    GFX_MALLOC_FREE(data);
#endif /* USE_MEM_CUSTOM */
}
