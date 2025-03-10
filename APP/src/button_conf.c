#include "button_conf.h"
#include "button.h"

const BUTTON_Typedef  ButtonConfigs[MULTI_BUTTON] = {
    {
        GPIOA,
        GPIO_PIN_1,
        GPIO_Mode_IPU,
        GPIO_PULLUP,
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
        GPIO_PULLUP
    },
    {
        GPIOA,
        GPIO_PIN_4,
        GPIO_Mode_IPU,
        GPIO_PULLUP,
    }
};
