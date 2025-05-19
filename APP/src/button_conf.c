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
