#include "gfx_hal_tick.h"

static gfx_hal_timer_t sys_time;
static volatile uint8_t tick_irq_flag;

/**
 * You have to call this function periodically
 * @param tick_period the call period of this function in milliseconds
 */
void gfx_tick_inc(gfx_hal_timer_t tick_period)
{
    tick_irq_flag = 0;
    sys_time = sys_time + tick_period;
}

/**
 * Get the elapsed milliseconds since start up
 * @return the elapsed milliseconds
 */
gfx_hal_timer_t gfx_tick_get(void)
{
#if USE_TICK_CUSTOM == 0
    gfx_hal_timer_t c_tick;

    #if 0
    do
    {
        tick_irq_flag = 1;
        c_tick = sys_time;
    } while (!tick_irq_flag);
    #endif /* nguyen.bui need review */

    c_tick = sys_time;

    return c_tick;
#else
    return GFX_TICK_CUSTOM_SYS_TIME_EXPR;
#endif
}

/**
 * Get the elapsed milliseconds since a previous time stamp
 * @param prev_tick a previous time stamp
 * @return the elapsed milliseconds since 'prev_tick'
 */
gfx_hal_timer_t gfx_tick_elaps(gfx_hal_timer_t prev_tick)
{
    gfx_hal_timer_t curr_tick = gfx_tick_get();

    if (curr_tick >= prev_tick)
    {
        prev_tick = curr_tick - prev_tick;
    }
    else
    {
        prev_tick = GFX_HAL_TIMER_MAX - prev_tick + 1;
        prev_tick = prev_tick + curr_tick; 
    }

    return prev_tick;
}

