#ifndef __GFX_LL_H__
#define __GFX_LL_H__

#include "gfx.h"

typedef uint8_t gfx_ll_node_t;

typedef struct {
    uint32_t       n_size;
    gfx_ll_node_t* head;
    gfx_ll_node_t* tail;
} gfx_ll_t;

void gfx_ll_init(gfx_ll_t* ll_p, uint32_t n_size);
void* gfx_ll_ins_head(gfx_ll_t* ll_p);
void* gfx_ll_ins_tail(gfx_ll_t* ll_p);
void gfx_ll_clear(gfx_ll_t* ll_p);
void gfx_ll_remove(gfx_ll_t* ll_p, gfx_ll_node_t* curr);
void gfx_ll_delete(gfx_ll_t* ll_p, gfx_ll_node_t* curr);
void* gfx_ll_get_head(const gfx_ll_t* ll_p);
void* gfx_ll_get_tail(const gfx_ll_t* ll_p);
void* gfx_ll_get_next(const gfx_ll_t* ll_p, const gfx_ll_node_t* curr);
void* gfx_ll_get_prev(const gfx_ll_t* ll_p, const gfx_ll_node_t* curr);

#endif /* __GFX_LL_H__ */

