#ifndef __BUTTON__
#define __BUTTON__

#include <stdint.h>
#include "button_conf.h"

#ifndef BUTTON_FILTER_ORDER
#define BUTTON_FILTER_ORDER 8
#endif /* BUTTON_FILTER_ORDER */

typedef enum
{
    STATUS_BUTTON_IDLE             = 0x00,
    STATUS_BUTTON_RELEASE          = 0x01,
    STATUS_BUTTON_PRESS            = 0x02,
    STATUS_BUTTON_CLICK            = 0x03,
    STATUS_BUTTON_DOUBLE_CLICK     = 0x04,
    STATUS_BUTTON_LONG_PRESS       = 0x05,
} BUTTON_STATUS;

typedef void (*CallBackFunction)(void);

typedef struct {
    CallBackFunction ClickFunc;
    CallBackFunction DoubleClickFunc;
    CallBackFunction ReleaseFunc;
    CallBackFunction LongPressFunc;  
} ButtonHook_Typedef;

typedef struct {
    uint8_t index;
    uint8_t releaseTime;
    uint8_t pullType;
    uint32_t pressTime;
    uint32_t lastPressTime;
    BUTTON_STATUS status;
    ButtonHook_Typedef ButtonHook;
    uint8_t filter[BUTTON_FILTER_ORDER];
} Debouncer_Typedef;

void EXTIConfig(void);
void ButtonConfig(void);
void ButtonProcess(void);
uint8_t ButtonPress(void);
uint8_t ButtonRelease(void);
BUTTON_STATUS ButtonClick(void);
void RegisterClickFunction(CallBackFunction __function);
void RegisterDoubleClickFunction(CallBackFunction __function);
void RegisterLongPressFunction(CallBackFunction __function);

#endif /* __BUTTON__ */
