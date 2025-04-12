#include "button.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define BUTTON_NUMBER               (4U)
#define BUTTON_DEBOUNCE_TIME        (50U)
#define BUTTON_RELEASE_TIME         (150U)
#define BUTTON_LONG_TIME            (800U)

#define BUTTON_NUMBER               (4U)
#define BUTTON_DEBOUNCE_TIME        (50U)
#define BUTTON_RELEASE_TIME         (150U)
#define BUTTON_LONG_TIME            (800U)

extern uint32_t TIM_GetTimerCount(void);

static BUTTON_TYPEDEF   Buttons[BUTTON_NUMBER];
static ButtonCallback   ButtonHook;
static const BUTTON_CONFIG  ButtonConfigs[BUTTON_NUMBER] = {
    {
        GPIOA,
        GPIO_PIN_0,
        GPIO_Mode_IPU,
        GPIO_PULLUP,
    },
    {
        GPIOA,
        GPIO_PIN_1,
        GPIO_Mode_IPU,
        GPIO_PULLUP
    },
    {
        GPIOA,
        GPIO_PIN_2,
        GPIO_Mode_IPU,
        GPIO_PULLUP
    },
    {
        GPIOA,
        GPIO_PIN_3,
        GPIO_Mode_IPU,
        GPIO_PULLUP,
    }
};

extern EventGroupHandle_t xButtonEventGroup;

void HandleButtonEvent(uint8_t evt, uint8_t btn)
{
    switch (btn)
    {
        case BUTTON_KEY_RIGHT:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR6 = 1;
                break;
                
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                xEventGroupSetBits(xButtonEventGroup, EVENT_BUTTON_KEY_RIGHT);
                break;

                default:
                break;
            }
        }
        break;

        case BUTTON_KEY_LEFT:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR7 = 1;
                break;
                
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR7 = 0;
                xEventGroupSetBits(xButtonEventGroup, EVENT_BUTTON_KEY_LEFT);
                break;
        
                default:
                break;
            }
        }
        break;

        case BUTTON_KEY_DOWN:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOB->ODR.BITS.ODR0 = 1;
                break;

                case BUTTON_RELEASE_EVENT:
                GPIOB->ODR.BITS.ODR0 = 0;
                xEventGroupSetBits(xButtonEventGroup, EVENT_BUTTON_KEY_DOWN);
                break;
        
                default:
                break;
            }
        }
        break;

        case BUTTON_KEY_UP:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR6 = 1;
                break;

                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                xEventGroupSetBits(xButtonEventGroup, EVENT_BUTTON_KEY_UP);
                break;
        
                default:
                break;
            }
        }
        break;

        default:
        break;
    }
}

static uint8_t ReadButton(uint8_t btn)
{
    return GPIO_ReadInputDataBit(ButtonConfigs[btn].Port, ButtonConfigs[btn].Pin) ^ Buttons[btn].pullType;
}

void RegisterButtonEvent(ButtonCallback __function)
{
    ButtonHook = __function;
}

void ButtonConfig(void)
{
    uint8_t i;
    GPIO_InitTypeDef GPIO_InitStruct;

    for (i = 0; i < BUTTON_NUMBER; ++i)
    {
        GPIO_InitStruct.GPIO_Pin = ButtonConfigs[i].Pin;
        GPIO_InitStruct.GPIO_Mode = ButtonConfigs[i].Mode;
        GPIO_Init(ButtonConfigs[i].Port, &GPIO_InitStruct);
        
        Buttons[i].index = i;
        Buttons[i].numClick = 0;
        Buttons[i].pullType = ButtonConfigs[i].PullMode;
        Buttons[i].state = STATUS_BUTTON_IDLE;
    }
}

/******************************************************
 
               +----------+
//===========> |   IDLE   |
//             +----------+
//                  //
//                  //
//                  ↓↓
//             +----------+        +----------+
//             |   DOWN   |   ==>  |  COUNTER |
//             +----------+        +----------+
//               //    ↑↑               //
//               //    //               //
//               ↓↓    //               ↓↓
+----------+        +----------+        +----------+
| RELEASE  |  <==   |    UP    |   <==  |   PRESS  |
+----------+        +----------+        +----------+
↑↑                                      //
//                                      //
//                                      ↓↓
//                                 +----------+
//                                 |   LONG   |
//================================ |   PRESS  |
                                   +----------+

**********************************************************/

void ButtonProcess(void)
{
    int btn = 0;
    uint8_t activeLevel = 0;

    for (btn = 0; btn < BUTTON_NUMBER; ++btn)
    {
        activeLevel = ReadButton(btn);

        switch (Buttons[btn].state)
        {
        case STATUS_BUTTON_IDLE:
            if (activeLevel)
            {
                Buttons[btn].state      = STATUS_BUTTON_DOWN;
                Buttons[btn].numClick   = 0;
                Buttons[btn].startTime  = xTaskGetTickCount();
            }
            break;
    
        case STATUS_BUTTON_DOWN:
            if (!activeLevel)
            {
                Buttons[btn].state      = STATUS_BUTTON_UP;
                Buttons[btn].startTime  = xTaskGetTickCount();
            }
            else
            {
                if (xTaskGetTickCount() - Buttons[btn].startTime > BUTTON_DEBOUNCE_TIME)
                {
                    Buttons[btn].state = STATUS_BUTTON_COUNTER;
                }
            }
            break;
    
        case STATUS_BUTTON_UP:
            if (activeLevel)
            {
                Buttons[btn].state      = STATUS_BUTTON_DOWN;
                Buttons[btn].startTime  = xTaskGetTickCount();
            }
            else
            {
                if (xTaskGetTickCount() - Buttons[btn].startTime > BUTTON_RELEASE_TIME)
                {
                    Buttons[btn].state = STATUS_BUTTON_RELEASE;
                    ButtonHook(BUTTON_RELEASE_EVENT, btn);
                }
            }
            break;
    
        case STATUS_BUTTON_COUNTER:
            Buttons[btn].state      = STATUS_BUTTON_PRESS;
            Buttons[btn].numClick   = Buttons[btn].numClick + 1;

            if (Buttons[btn].numClick == 1)
            {
                ButtonHook(BUTTON_CLICK_EVENT, btn);
            }

            break;
    
        case STATUS_BUTTON_PRESS:
            if (!activeLevel)
            {
                Buttons[btn].state      = STATUS_BUTTON_UP;
                Buttons[btn].startTime  = xTaskGetTickCount();
            }
            else
            {
                if (xTaskGetTickCount() - Buttons[btn].startTime > BUTTON_LONG_TIME)
                {
                    Buttons[btn].state = STATUS_BUTTON_LONG_PRESS;
                    ButtonHook(BUTTON_LONG_PRESS_EVENT, btn);
                }
            }
    
            break;
    
        case STATUS_BUTTON_LONG_PRESS:
            if (!activeLevel)
            {
                Buttons[btn].state = STATUS_BUTTON_RELEASE;
                ButtonHook(BUTTON_RELEASE_EVENT, btn);
            }
            break;
    
        case STATUS_BUTTON_RELEASE:
            Buttons[btn].startTime  = xTaskGetTickCount();
            Buttons[btn].numClick   = 0;
            Buttons[btn].state      = STATUS_BUTTON_IDLE;
            break;
        
        default:
            Buttons[btn].state = STATUS_BUTTON_IDLE;
            break;
        }
    }
}
