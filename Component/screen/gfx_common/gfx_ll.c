#include <string.h>
#include "gfx_ll.h"
#include "gfx_alloc.h"

#define LL_NODE_META_SIZE(ll_p)     (sizeof(gfx_ll_node_t*) + sizeof(gfx_ll_node_t*))     
#define LL_PREV_OFFSET_P(ll_p)      ((ll_p)->n_size + sizeof(gfx_ll_node_t*))
#define LL_NEXT_OFFSET_P(ll_p)      (LL_PREV_OFFSET_P(ll_p) + sizeof(gfx_ll_node_t*))

static void node_set_next(gfx_ll_t* ll_p, gfx_ll_node_t* curr, gfx_ll_node_t* next);
static void node_set_prev(gfx_ll_t* ll_p, gfx_ll_node_t* curr, gfx_ll_node_t* prev);

void gfx_ll_init(gfx_ll_t* ll_p, uint32_t n_size)
{
    ll_p->head = NULL;
    ll_p->tail = NULL;

    /* Căn chỉnh theo 4 byte */
    if (n_size & 0x03)
    {
        n_size = n_size & (~0x03);
        n_size = n_size + 0x04;
    }

    ll_p->n_size = n_size;
}

void* gfx_ll_ins_head(gfx_ll_t* ll_p)
{
    void* new_p = gfx_mem_alloc(ll_p->n_size + LL_NODE_META_SIZE(ll_p));

    if (new_p)
    {
        node_set_prev(ll_p, new_p, NULL);
        node_set_next(ll_p, new_p, ll_p->head);

        if (ll_p->head != NULL)
        {
            node_set_prev(ll_p, ll_p->head, new_p);
        }

        ll_p->head = new_p;

        if (ll_p->tail == NULL)
        {
            ll_p->tail = new_p;
        }
    }

    return new_p;
}

void* gfx_ll_ins_tail(gfx_ll_t* ll_p)
{
    gfx_ll_node_t* new_p = gfx_mem_alloc(ll_p->n_size + LL_NODE_META_SIZE(ll_p));

    if (new_p)
    {
        node_set_prev(ll_p, new_p, ll_p->tail);
        node_set_next(ll_p, new_p, NULL);

        if (ll_p->tail != NULL)
        {
            node_set_next(ll_p, ll_p->tail, new_p);
        }

        ll_p->tail = new_p;

        if (ll_p->head == NULL)
        {
            ll_p->head = new_p;
        }
    }

    return new_p;
}

void gfx_ll_remove(gfx_ll_t* ll_p, gfx_ll_node_t* curr)
{
    if (gfx_ll_get_head(ll_p) == curr)
    {
        ll_p->head = gfx_ll_get_next(ll_p, curr);
        if (ll_p->head == NULL)
            ll_p->tail = NULL;
        else
            node_set_prev(ll_p, ll_p->head, NULL);
    }
    else if (gfx_ll_get_tail(ll_p) == curr)
    {
        ll_p->tail = gfx_ll_get_prev(ll_p, curr);
        if (ll_p->tail == NULL)
            ll_p->head = NULL;
        else
            node_set_next(ll_p, ll_p->tail, NULL);
    }
    else
    {
        gfx_ll_node_t* n_prev = gfx_ll_get_prev(ll_p, curr);
        gfx_ll_node_t* n_next = gfx_ll_get_next(ll_p, curr);

        node_set_next(ll_p, n_prev, n_next);
        node_set_prev(ll_p, n_next, n_prev);
    }
}

void gfx_ll_delete(gfx_ll_t* ll_p, gfx_ll_node_t* curr)
{
    gfx_ll_remove(ll_p, curr);
    gfx_mem_free(curr);
}

void gfx_ll_clear(gfx_ll_t* ll_p)
{
    void* n_curr;
    void* n_next;

    n_next = NULL;
    n_curr = gfx_ll_get_head(ll_p);

    while (n_curr != NULL)
    {
        n_next = gfx_ll_get_next(ll_p, n_curr);
        gfx_ll_delete(ll_p, n_curr);
        n_curr = n_next;
    }
}

void* gfx_ll_get_head(const gfx_ll_t* ll_p)
{
    void* head = NULL;

    if (ll_p != NULL)
    {
        head = ll_p->head;
    }

    return head;
}

void* gfx_ll_get_tail(const gfx_ll_t* ll_p)
{
    void* tail = NULL;

    if (ll_p != NULL)
    {
        tail = ll_p->tail;
    }

    return tail;
}

void* gfx_ll_get_next(const gfx_ll_t* ll_p, const gfx_ll_node_t* curr)
{
    void* next = NULL;

    if (ll_p != NULL)
    {
        memcpy(next, curr + LL_NEXT_OFFSET_P(ll_p), sizeof(void*));
    }

    return next;
}

void* gfx_ll_get_prev(const gfx_ll_t* ll_p, const gfx_ll_node_t* curr)
{
    void* prev = NULL;

    if (ll_p != NULL)
    {
        memcpy(prev, curr + LL_PREV_OFFSET_P(ll_p), sizeof(void*));
    }

    return prev;
}

static void node_set_next(gfx_ll_t* ll_p, gfx_ll_node_t* curr, gfx_ll_node_t* next)
{
    if (curr == NULL) return;

    uint32_t node_p_size = sizeof(gfx_ll_node_t*);
    if (next) memcpy(curr + LL_NEXT_OFFSET_P(ll_p), next, node_p_size);
    else memset(curr + LL_NEXT_OFFSET_P(ll_p), 0, node_p_size);
}

static void node_set_prev(gfx_ll_t* ll_p, gfx_ll_node_t* curr, gfx_ll_node_t* prev)
{
    if (curr == NULL) return;

    uint32_t node_p_size = sizeof(gfx_ll_node_t*);
    if (prev) memcpy(curr + LL_PREV_OFFSET_P(ll_p), prev, node_p_size);
    else memset(curr + LL_PREV_OFFSET_P(ll_p), 0, node_p_size);
}
