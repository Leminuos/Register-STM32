#ifndef __GFX_HAL_TICK_H__
#define __GFX_HAL_TICK_H__

#include <gfx.h>

#ifndef USE_TICK_CUSTOM
#define USE_TICK_CUSTOM                 0
#endif /* USE_TICK_CUSTOM */

typedef uint32_t    gfx_hal_timer_t;

#define GFX_HAL_TIMER_MAX       UINT32_MAX

gfx_hal_timer_t gfx_tick_get(void);
void gfx_tick_inc(gfx_hal_timer_t tick_period);
gfx_hal_timer_t gfx_tick_elaps(gfx_hal_timer_t prev_tick);

#endif /* __GFX_HAL_TICK_H__ */

