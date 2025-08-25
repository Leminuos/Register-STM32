#include <string.h>
#include "gfx_alloc.h"

void* gfx_mem_alloc(uint32_t size)
{
    void* alloc = NULL;

#if USE_MEM_CUSTOM
    alloc = malloc(size);
    memset(alloc, 0, size);
#else
#endif

    return alloc;
}

