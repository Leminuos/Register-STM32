#include <string.h>
#include "gfx_ll.h"
#include "gfx_alloc.h"

#define LL_NODE_META_SIZE(ll_p)     (sizeof(gfx_ll_node_t*) + sizeof(gfx_ll_node_t*))     
#define LL_PREV_OFFSET_P(ll_p)      ((ll_p)->n_size)
#define LL_NEXT_OFFSET_P(ll_p)      ((ll_p)->n_size + sizeof(gfx_ll_node_t*))

static void node_set_next(gfx_ll_t* ll_p, gfx_ll_node_t* curr, gfx_ll_node_t* next);
static void node_set_prev(gfx_ll_t* ll_p, gfx_ll_node_t* curr, gfx_ll_node_t* prev);


/**
 * Initialize linked list
 * @param ll_p pointer to linked list
 * @param n_size the size of 1 node in bytes
 * @return void
 */
void gfx_ll_init(gfx_ll_t* ll_p, uint32_t n_size)
{
    ll_p->head = NULL;
    ll_p->tail = NULL;

    /* Round the size up to 4 */
    if (n_size & 0x03)
    {
        n_size = n_size & (~0x03);
        n_size = n_size + 0x04;
    }

    ll_p->n_size = n_size;
}

/**
 * Add a new head to a linked list
 * @param ll_p pointer to linked list.
 * @return pointer to new head. 
 */
void* gfx_ll_ins_head(gfx_ll_t* ll_p)
{
    // Allocate a new node: data node, pointer to next node, pointer to previos node
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

/**
 * Add a new tail to a linked list
 * @param ll_p pointer to linked list.
 * @return pointer to new tail. 
 */
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

/**
 * Remove the node 'node_p' from 'll_p' linked list.
 * It does not free the the memory of node.
 * @param ll_p pointer to the linked list of 'node_p'
 * @param node_p pointer to node in 'll_p' linked list
 * @return void
 */
void gfx_ll_remove(gfx_ll_t* ll_p, void* node_p)
{
    if (gfx_ll_get_head(ll_p) == node_p)
    {
        ll_p->head = gfx_ll_get_next(ll_p, node_p);
        if (ll_p->head == NULL)
            ll_p->tail = NULL;
        else
            node_set_prev(ll_p, ll_p->head, NULL);
    }
    else if (gfx_ll_get_tail(ll_p) == node_p)
    {
        ll_p->tail = gfx_ll_get_prev(ll_p, node_p);
        if (ll_p->tail == NULL)
            ll_p->head = NULL;
        else
            node_set_next(ll_p, ll_p->tail, NULL);
    }
    else
    {
        gfx_ll_node_t* n_prev = gfx_ll_get_prev(ll_p, node_p);
        gfx_ll_node_t* n_next = gfx_ll_get_next(ll_p, node_p);

        node_set_next(ll_p, n_prev, n_next);
        node_set_prev(ll_p, n_next, n_prev);
    }
}

/**
 * Delete the node 'node_p' from 'll_p' linked list.
 * It free the the memory of node.
 * @param ll_p pointer to the linked list of 'node_p'
 * @param node_p pointer to node in 'll_p' linked list
 * @return void
 */
void gfx_ll_delete(gfx_ll_t* ll_p, void* node_p)
{
    gfx_ll_remove(ll_p, node_p);
    gfx_mem_free(node_p);
}

/**
 * Remove and free all elements from a linked list. The list remain valid but become empty.
 * @param ll_p pointer to linked list
 * @return void
 */
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

/**
 * Move a node to a new linked list
 * @param ll_ori_p pointer to the old linked list
 * @param ll_new_p pointer to the new linked list
 * @param node_p pointer to a node
 */
void lv_ll_chg_list(gfx_ll_t * ll_old_p, gfx_ll_t * ll_new_p, void* node_p)
{
    gfx_ll_remove(ll_old_p, node_p);

    node_set_prev(ll_new_p, node_p, NULL);
    node_set_next(ll_new_p, node_p, ll_new_p->head);

    if (ll_new_p->head != NULL)
    {
        node_set_prev(ll_new_p, ll_new_p->head, node_p);
    }

    ll_new_p->head = node_p;
    if (ll_new_p->tail == NULL)
    {
        ll_new_p->tail = node_p;
    }
}

/**
 * Return with head node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the head of 'll_p'
 */
void* gfx_ll_get_head(const gfx_ll_t* ll_p)
{
    void* head = NULL;

    if (ll_p != NULL)
    {
        head = ll_p->head;
    }

    return head;
}

/**
 * Return with tail node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the tail of 'll_p'
 */
void* gfx_ll_get_tail(const gfx_ll_t* ll_p)
{
    void* tail = NULL;

    if (ll_p != NULL)
    {
        tail = ll_p->tail;
    }

    return tail;
}

/**
 * Return with the pointer of the next node after 'n_curr'
 * @param ll_p pointer to linked list
 * @param n_curr pointer a current node
 * @return pointer to the next node
 */
void* gfx_ll_get_next(const gfx_ll_t* ll_p, const void* n_curr)
{
    void* next = NULL;

    if (ll_p != NULL)
    {
        memcpy(&next, ((gfx_ll_node_t*)n_curr) + LL_NEXT_OFFSET_P(ll_p), sizeof(void*));
    }

    return next;
}

/**
 * Return with the pointer of the previous node after 'n_curr'
 * @param ll_p pointer to linked list
 * @param n_curr pointer a current node
 * @return pointer to the previous node
 */
void* gfx_ll_get_prev(const gfx_ll_t* ll_p, const void* n_curr)
{
    void* prev = NULL;

    if (ll_p != NULL)
    {
        memcpy(&prev, ((gfx_ll_node_t*)n_curr) + LL_PREV_OFFSET_P(ll_p), sizeof(void*));
    }

    return prev;
}

/**
 * Set the next node pointer of a node
 * @param ll_p pointer to linked list
 * @param n_curr pointer to a node which next node pointer should be set
 * @param next pointer to a node which should be the next node before 'n_curr'
 * @return void
 */
static void node_set_next(gfx_ll_t* ll_p, gfx_ll_node_t* n_curr, gfx_ll_node_t* next)
{
    if (n_curr == NULL) return;

    uint32_t node_p_size = sizeof(gfx_ll_node_t*);
    if (next) memcpy(n_curr + LL_NEXT_OFFSET_P(ll_p), &next, node_p_size);
    else memset(n_curr + LL_NEXT_OFFSET_P(ll_p), 0, node_p_size);
}

/**
 * Set the pervious node pointer of a node
 * @param ll_p pointer to linked list
 * @param n_curr pointer to a node which prev. node pointer should be set
 * @param prev pointer to a node which should be the previous node before 'n_curr'
 * @return void
 */
static void node_set_prev(gfx_ll_t* ll_p, gfx_ll_node_t* n_curr, gfx_ll_node_t* prev)
{
    if (n_curr == NULL) return;

    uint32_t node_p_size = sizeof(gfx_ll_node_t*);
    if (prev) memcpy(n_curr + LL_PREV_OFFSET_P(ll_p), &prev, node_p_size);
    else memset(n_curr + LL_PREV_OFFSET_P(ll_p), 0, node_p_size);
}
