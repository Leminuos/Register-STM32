#include "main.h"
#include "gfx_obj.h"
#include "gfx_task.h"

extern void TraceInit(void);

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    TestLed();
    gfx_init();
}

void loop(void)
{
    gfx_task_handler();
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}
