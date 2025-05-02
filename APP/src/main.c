#include "main.h"

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

void vButtonTask(void* pvParameters)
{
    while (1)
    {
        ButtonProcess();
        HID_SendCommandList();
        vTaskDelay(5);
    }
}

void init(void)
{
    setupHardware();
    TimerConfig();
    TestLed();
    USB_PowerOnReset();
    ButtonConfig();
    RegisterButtonEvent(HandleButtonEvent);

    BaseType_t xReturned;

    xReturned = xTaskCreate(vButtonTask,
                            "Button Task",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            1 | portPRIVILEGE_BIT,
                            NULL
                        );
    
    if (xReturned == pdFAIL)
    {
        return;
    }

    vTaskStartScheduler();
}

void loop(void)
{
}
