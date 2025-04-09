#include "button_conf.h"
#include "button.h"

const BUTTON_CONFIG  ButtonConfigs[BUTTON_NUMBER] = {
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

void HandleButtonEvent(uint8_t evt, uint8_t btn)
{
    switch (btn)
    {
        case 0:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR6 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                break;
        
                case BUTTON_LONG_PRESS_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                GPIOA->ODR.BITS.ODR7 = 1;
                break;
        
                default:
        
                break;
            }
        }
        break;

        case 1:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR7 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR7 = 0;
                break;
        
                default:
        
                break;
            }
        }
        break;

        case 2:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOB->ODR.BITS.ODR0 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOB->ODR.BITS.ODR0 = 0;
                break;
        
                default:
        
                break;
            }
        }
        break;

        case 3:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR6 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
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
