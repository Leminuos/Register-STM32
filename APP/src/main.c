#include "main.h"

extern void delay(uint32_t mDelay);
void SystickConfig(uint32_t u32Reload);

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
    BootloaderInit();
}

void loop(void)
{
    if (GPIOA->IDR.BITS.IDR0 == 0)
    {
        delay(20);

        if (GPIOA->IDR.BITS.IDR0 == 0)
        {
            BootloaderProcess();
        }
    }
    else
    {
        delay(20);

        if (GPIOA->IDR.BITS.IDR0 == 1)
        {
            // Jump to application
            JumpToApplication();
        }
    }
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}
