#ifndef __BUTTON__
#define __BUTTON__

#include <stdint.h>
#include "button_conf.h"
#include "stm32_driver_gpio.h"

#define GPIO_NOPULL                 0x00
#define GPIO_PULLUP                 0x01
#define GPIO_PULLDOWN               0x02

/* Button event */
#define BUTTON_CLICK_EVENT          0x01
#define BUTTON_DOUBLE_CLICK_EVENT   0x02
#define BUTTON_LONG_PRESS_EVENT     0x03
#define BUTTON_RELEASE_EVENT        0x04

typedef void (*ButtonCallback)(uint8_t evt, uint8_t btn);

typedef enum
{
    STATUS_BUTTON_IDLE             = 0x00,
    STATUS_BUTTON_UP               = 0x01,
    STATUS_BUTTON_DOWN             = 0x02,
    STATUS_BUTTON_COUNTER          = 0x03,
    STATUS_BUTTON_PRESS            = 0x04,
    STATUS_BUTTON_LONG_PRESS       = 0x05,
    STATUS_BUTTON_RELEASE          = 0x06,
} BUTTON_STATE;

typedef struct {
    GPIO_Typedef*       Port;
    uint16_t            Pin;
    GPIOMode_TypeDef    Mode; 
    uint16_t            PullMode;
} BUTTON_CONFIG;

typedef struct {
    uint8_t                 index;
    uint8_t                 numClick;
    uint8_t                 pullType;
    uint32_t                startTime;
    BUTTON_STATE            state;
} BUTTON_TYPEDEF;

extern const BUTTON_CONFIG ButtonConfigs[BUTTON_NUMBER];

void EXTIConfig(void);
void ButtonConfig(void);
void ButtonProcess(void);
void RegisterButtonEvent(ButtonCallback __function);

#endif /* __BUTTON__ */
