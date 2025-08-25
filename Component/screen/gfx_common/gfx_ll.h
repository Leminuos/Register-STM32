#ifndef __GFX_LL_H__
#define __GFX_LL_H__

#include "gfx.h"

typedef uint8_t gfx_ll_node_t;

typedef struct {
    uint32_t       n_size;
    gfx_ll_node_t* head;
    gfx_ll_node_t* tail;
} gfx_ll_t;

#endif /* __GFX_LL_H__ */

