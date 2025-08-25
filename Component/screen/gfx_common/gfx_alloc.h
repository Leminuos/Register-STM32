#ifndef __GFX_ALLOC_H_
#define __GFX_ALLOC_H_

#include "gfx.h"

extern void* gfx_mem_alloc(uint32_t size);
extern void gfx_mem_free(void* data);

#endif /* __GFX_ALLOC_H_ */

