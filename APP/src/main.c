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
        vTaskDelay(5);
    }
}

extern const FontTypedef Font_14x15;
EventGroupHandle_t xButtonEventGroup;
TaskHandle_t ScreenHandle;

void vLogicalTask(void* pvParameters)
{
    EventBits_t uxBits;

    while (1)
    {
        uxBits = xEventGroupWaitBits(\
                    xButtonEventGroup,\
                    EVENT_BUTTON_ALL,\
                    pdTRUE,\
                    pdFALSE,\
                    portMAX_DELAY\
                );

        if (uxBits && GetStateSnake() == SNAKE_START_STATE)
        {
            SetStateSnake(SNAKE_RUN_STATE);
            vTaskResume(ScreenHandle);
        }

        if (uxBits & EVENT_BUTTON_KEY_UP)
        {
            SnakeUpdateDirection(BUTTON_KEY_UP);
        }

        if (uxBits & EVENT_BUTTON_KEY_DOWN)
        {
            SnakeUpdateDirection(BUTTON_KEY_DOWN);
        }

        if (uxBits & EVENT_BUTTON_KEY_LEFT)
        {
            SnakeUpdateDirection(BUTTON_KEY_LEFT);
        }

        if (uxBits & EVENT_BUTTON_KEY_RIGHT)
        {
            SnakeUpdateDirection(BUTTON_KEY_RIGHT);
        }
    }
}

void vScreenTask(void* pvParameters)
{
    uint32_t wait = 0;

    SnakeInit(&(SnakePixel) {
        .size = 8,
        .dir = 0xFF,
        .axes = {0, 0},
        .front.color = {0x07, 0xE0},
        .back.color = {0xFF, 0xFF},
        .target = {0xF8, 0x00}
    });

    while (1)
    {
        wait = SnakeOperation();

        if (wait == 0xFFFFFFFF)
        {
            vTaskSuspend(ScreenHandle);
        }
        else
        {
            vTaskDelay(wait);
        }
    }
}

void init(void)
{
    setupHardware();
    TimerConfig();
    ADC_Init();
    TestLed();
    ButtonConfig();
    RegisterButtonEvent(HandleButtonEvent);                            
    LCD_Create();

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

    xReturned = xTaskCreate(vLogicalTask,
                            "Logical Task",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            1 | portPRIVILEGE_BIT,
                            NULL
                        );

    if (xReturned == pdFAIL)
    {
        return;
    }

    xReturned = xTaskCreate(vScreenTask,
        "Screen Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        1 | portPRIVILEGE_BIT,
        &ScreenHandle
    );

    if (xReturned == pdFAIL)
    {
        return;
    }

    xButtonEventGroup = xEventGroupCreate();

    if (xButtonEventGroup == NULL)
    {
        printf("Can't not allocate event group for button event\n");
        return;
    }

    vTaskStartScheduler();
}

void loop(void)
{
}
