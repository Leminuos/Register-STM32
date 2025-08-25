#ifndef __GFX_TASK_H__
#define __GFX_TASK_H__


#include "gfx.h"

typedef struct {
    uint32_t period;
    uint32_t last_run;
    void (*task_func)(void*);
    uint8_t* task_name;
    uint8_t prio;
    uint8_t once;
} gfx_task_t;

void gfx_task_init(void);

#endif /* __GFX_TASK_H__ */

