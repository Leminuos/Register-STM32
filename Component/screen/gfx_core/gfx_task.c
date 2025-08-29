#include "gfx_task.h"
#include "gfx_ll.h"
#include "gfx_log.h"

static bool gfx_task_run;
static bool task_deleted;
#if 0
static bool task_handler_mutex;
#endif /* nguyen.bui need review */ 
static gfx_ll_t task_pri_queue[TASK_PRIO_MAX_SIZE];

static void gfx_task_exec(gfx_task_t* task_p);

/**
 * Init the gfx task module
 */
void gfx_task_init(void)
{
    uint8_t pri = 0;

    for (pri = 1; pri <= TASK_PRIO_MAX_SIZE; ++pri)
    {
        gfx_ll_init(&task_pri_queue[pri - 1], sizeof(gfx_task_t));
    }

    gfx_task_run = true;
}

/**
 * Create a new gfx_task
 * @param task_name name of task
 * @param task a function which is the task itself
 * @param period call period in ms unit
 * @param prio priority of the task
 * @return pointer to the new task
 */
gfx_task_t* gfx_task_create(char* task_name, gfx_task_func task_func, gfx_task_timer_t period, gfx_task_prio_t prio, void* task_param)
{
    gfx_task_t* new_gfx_task = NULL;

    new_gfx_task = gfx_ll_ins_head(&task_pri_queue[prio - 1]);
    if (new_gfx_task == NULL) return NULL;

    new_gfx_task->once       = 0;
    new_gfx_task->period     = period;
    new_gfx_task->prio       = prio;
    new_gfx_task->task_param = task_param;
    new_gfx_task->last_run   = gfx_tick_get();
    new_gfx_task->task_name  = task_name;
    new_gfx_task->task_func  = task_func;

    return new_gfx_task;
}

/**
 * Delete a gfx_task
 * @param gfx_task_p pointer to task created by gfx_task_p
 */
void gfx_task_delete(gfx_task_t* gfx_task_p)
{
    gfx_task_prio_t prio_task = 0;
    gfx_ll_t* ll_task = NULL;

    prio_task = gfx_task_p->prio;
    ll_task   = &task_pri_queue[prio_task - 1];

    gfx_ll_delete(ll_task, gfx_task_p);
}

/**
 * Set new priority for a gfx_task
 * @param gfx_task_p pointer to a gfx_task
 * @param new_prio the new priority
 */
void gfx_task_set_prio(gfx_task_t* gfx_task_p, gfx_task_prio_t new_prio)
{
    gfx_task_prio_t old_prio = 0;
    gfx_ll_t* old_ll = NULL;
    gfx_ll_t* new_ll = NULL;

    old_prio = gfx_task_p->prio;
    if (new_prio == GFX_TASK_PRIO_OFF || old_ll == new_ll) return;

    old_ll = &task_pri_queue[old_prio - 1];
    new_ll = &task_pri_queue[new_prio - 1];
    lv_ll_chg_list(old_ll, new_ll, gfx_task_p);
}

/**
 * Call it  periodically to handle lv_tasks.
 */
void gfx_task_handler(void)
{    
    if (gfx_task_run == false) return;

    gfx_task_prio_t pri = 0;
    void* task_target   = NULL;
    void* task_next     = NULL;

    #if 0
    if (task_handler_mutex) return;
    task_handler_mutex = true;
    #endif /* nguyen.bui need review */

    for (pri = TASK_PRIO_MAX_SIZE; pri >= 1; --pri)
    { 
        task_target = gfx_ll_get_head(&task_pri_queue[pri - 1]);
        if (task_target == NULL) continue;
        while (task_target != NULL)
        {
            task_deleted = false;
            task_next = gfx_ll_get_next(&task_pri_queue[pri - 1], task_target);
            gfx_task_exec(task_target);

            if (task_deleted != false) break;
            task_target = task_next;
        }
    }

    #if 0
    task_handler_mutex = false;
    #endif /* nguyen.bui need review */
}

static void gfx_task_exec(gfx_task_t* task_p)
{
    gfx_hal_timer_t period = gfx_tick_elaps(task_p->last_run);

    if (period >= task_p->period)
    {
        task_deleted = false;
        task_p->last_run = gfx_tick_get();
        task_p->task_func(task_p->task_param);

        if (task_p->once != 0)
        {
            task_deleted = true;
            gfx_task_delete(task_p);
        }
    }
}
