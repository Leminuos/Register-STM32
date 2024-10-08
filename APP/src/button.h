#ifndef __BUTTON__
#define __BUTTON__

#include <stdint.h>
#include "button_conf.h"

#ifndef BUTTON_FILTER_ORDER
#define BUTTON_FILTER_ORDER 8
#endif /* BUTTON_FILTER_ORDER */

typedef enum
{
    BUTTON_RELEASE          = 0x00,
    BUTTON_PRESS            = 0x01,
    BUTTON_IDLE             = 0xFF,
} BUTTON_STATUS;

typedef struct {
    uint8_t index;
    uint8_t releaseTime;
    uint8_t pullType;
    uint32_t pressTime;
    uint32_t lastPressTime;
    BUTTON_STATUS status;
    BUTTON_STATUS lastStatus;
    uint8_t filter[BUTTON_FILTER_ORDER];
} Debouncer_Typedef;

void EXTIConfig(void);
void ButtonConfig(void);
void ButtonProcess(void);
uint8_t ButtonPress(void);
uint8_t ButtonRelease(void);
uint8_t ButtonClick(void);

#endif /* __BUTTON__ */
