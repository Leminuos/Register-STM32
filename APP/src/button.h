#ifndef __BUTTON__
#define __BUTTON__

#include <stdint.h>
#include "button_conf.h"
#include "stm32_driver_gpio.h"

#ifndef BUTTON_FILTER_ORDER
#define BUTTON_FILTER_ORDER 8
#endif /* BUTTON_FILTER_ORDER */

#define GPIO_NOPULL         0x00
#define GPIO_PULLUP         0x01
#define GPIO_PULLDOWN       0x02

typedef void (*CallBackFunction)(uint8_t);

typedef enum
{
    STATUS_BUTTON_IDLE             = 0x00,
    STATUS_BUTTON_RELEASE          = 0x01,
    STATUS_BUTTON_PRESS            = 0x02,
    STATUS_BUTTON_CLICK            = 0x03,
    STATUS_BUTTON_DOUBLE_CLICK     = 0x04,
    STATUS_BUTTON_LONG_PRESS       = 0x05,
} BUTTON_STATUS;

typedef struct {
    GPIO_Typedef*       Port;
    uint16_t            Pin;
    GPIOMode_TypeDef    Mode; 
    uint16_t            PullMode;
} BUTTON_Typedef;

typedef struct {
    CallBackFunction ClickFunc;
    CallBackFunction DoubleClickFunc;
    CallBackFunction ReleaseFunc;
    CallBackFunction LongPressFunc;  
} ButtonHook_Typedef;

typedef struct {
    uint8_t                 index;
    uint8_t                 nClicks;
    uint8_t                 releaseTime;
    uint8_t                 pullType;
    uint32_t                pressTime;
    uint32_t                lastPressTime;
    BUTTON_STATUS           status;
    uint8_t                 filter[BUTTON_FILTER_ORDER];
} Debouncer_Typedef;

extern const BUTTON_Typedef  ButtonConfigs[MULTI_BUTTON];

void EXTIConfig(void);
void ButtonConfig(void);
void ButtonProcess(void);
uint8_t ButtonPress(uint8_t idxbtn);
uint8_t ButtonRelease(uint8_t idxbtn);
BUTTON_STATUS ButtonClick(uint8_t idxbtn);
void RegisterClickFunction(CallBackFunction __function);
void RegisterDoubleClickFunction(CallBackFunction __function);
void RegisterLongPressFunction(CallBackFunction __function);

#endif /* __BUTTON__ */
