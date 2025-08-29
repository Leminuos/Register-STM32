#ifndef __GFX_ALLOC_H_
#define __GFX_ALLOC_H_

#include "gfx.h"

#ifndef USE_MEM_CUSTOM
#define USE_MEM_CUSTOM          0
#endif /* USE_MEM_CUSTOM */

extern void gfx_mem_init(void);
extern void gfx_mem_free(void* data);
extern void* gfx_mem_alloc(uint32_t size);

#endif /* __GFX_ALLOC_H_ */

