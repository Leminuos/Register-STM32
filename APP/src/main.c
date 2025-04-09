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
    TestLed();
    ButtonConfig();
    RegisterButtonEvent(HandleButtonEvent);                            
    LCD_Create();
}

void loop(void)
{
}
