#ifndef __GFX_TASK_H__
#define __GFX_TASK_H__

#include "gfx.h"
#include "gfx_hal_tick.h"

enum
{
    GFX_TASK_PRIO_OFF = 0,
    GFX_TASK_PRIO_LOWEST,
    GFX_TASK_PRIO_LOW,
    GFX_TASK_PRIO_MID,
    GFX_TASK_PRIO_HIGH,
    GFX_TASK_PRIO_HIGHEST,
    GFX_TASK_PRIO_NUM,
};
typedef uint8_t gfx_task_prio_t;
typedef uint32_t gfx_task_timer_t;
typedef void(*gfx_task_func)(void*);

typedef struct {
    char* task_name;
    void* task_param;
    gfx_task_timer_t period;
    gfx_task_timer_t last_run;
    gfx_task_func task_func;
    gfx_task_prio_t prio;
    uint8_t once;
} gfx_task_t;

/**
 * Delete the gfx_task after one call
 * @param gfx_task_p pointer to a gfx_task.
 */
static void gfx_task_once(gfx_task_t * gfx_task_p)
{
    gfx_task_p->once = 1;
}

/**
 * Make a gfx_task ready. It will not wait its period.
 * @param gfx_task_p pointer to a gfx_task.
 */
static void gfx_task_ready(gfx_task_t * gfx_task_p)
{
    gfx_task_p->last_run = gfx_tick_get() - gfx_task_p->period - 1;
}

/**
 * Set new period for a gfx_task
 * @param gfx_task_p pointer to a gfx_task
 * @param period the new period
 */
static void gfx_task_set_period(gfx_task_t* gfx_task_p, uint32_t period)
{
    gfx_task_p->period = period;
}

/**
 * Reset a gfx_task.
 * It will be called the previously set period milliseconds later.
 * @param gfx_task_p pointer to a gfx_task.
 */
static void gfx_task_reset(gfx_task_t* gfx_task_p)
{
    gfx_task_p->last_run = gfx_tick_get();
}

void gfx_task_init(void);
void gfx_task_handler(void);
void gfx_task_delete(gfx_task_t* gfx_task_p);
void gfx_task_set_prio(gfx_task_t* gfx_task_p, gfx_task_prio_t new_prio);
gfx_task_t* gfx_task_create(char* task_name, gfx_task_func task_func, gfx_task_timer_t period, gfx_task_prio_t prio, void* task_param);

#endif /* __GFX_TASK_H__ */

