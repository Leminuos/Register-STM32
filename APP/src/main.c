#include "main.h"

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

extern void EthernetInit(void);

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    TimerConfig();
    TestLed();
    EthernetInit();
}

void loop(void)
{
    
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}
