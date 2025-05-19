#include "main.h"

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
    TimerConfig();
    TestLed();
    USB_PowerOnReset();
    HID_SendCommandList();
    ButtonConfig();
    RegisterButtonEvent(HandleButtonEvent);
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
