#include "stm32f103.h"
#include "button.h"

static BUTTON_TYPEDEF   Buttons[BUTTON_NUMBER];
static ButtonCallback   ButtonHook;

extern uint32_t TIM_GetTimerCount(void);

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
                Buttons[btn].startTime  = TIM_GetTimerCount();
            }
            break;
    
        case STATUS_BUTTON_DOWN:
            if (!activeLevel)
            {
                Buttons[btn].state      = STATUS_BUTTON_UP;
                Buttons[btn].startTime  = TIM_GetTimerCount();
            }
            else
            {
                if (TIM_GetTimerCount() - Buttons[btn].startTime > BUTTON_DEBOUNCE_TIME)
                {
                    Buttons[btn].state = STATUS_BUTTON_COUNTER;
                }
            }
            break;
    
        case STATUS_BUTTON_UP:
            if (activeLevel)
            {
                Buttons[btn].state      = STATUS_BUTTON_DOWN;
                Buttons[btn].startTime  = TIM_GetTimerCount();
            }
            else
            {
                if (TIM_GetTimerCount() - Buttons[btn].startTime > BUTTON_RELEASE_TIME)
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
                Buttons[btn].startTime  = TIM_GetTimerCount();
            }
            else
            {
                if (TIM_GetTimerCount() - Buttons[btn].startTime > BUTTON_LONG_TIME)
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
            Buttons[btn].startTime  = TIM_GetTimerCount();
            Buttons[btn].numClick   = 0;
            Buttons[btn].state      = STATUS_BUTTON_IDLE;
            break;
        
        default:
            Buttons[btn].state = STATUS_BUTTON_IDLE;
            break;
        }
    }
}
