#include "main.h"

void TestLed(void);
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
    TestLed();
    USB_PowerOnReset();
    ButtonConfig();
    RegisterButtonEvent(HandleButtonEvent);
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}

void TestLed(void)
{
    RCC->APB2ENR.BITS.IOPCEN = SET;
    
    GPIOC->CRH.BITS.MODE13 = 0x03;
    GPIOC->CRH.BITS.CNF13 = 0x00;
    GPIOC->ODR.BITS.ODR13 = 1;
}

void loop(void)
{
}
