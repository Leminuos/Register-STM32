#ifndef __BUTTON_CONFIG__
#define __BUTTON_CONFIG__

#include <stdint.h>

#define BUTTON_NUMBER               (4U)
#define BUTTON_DEBOUNCE_TIME        (50U)
#define BUTTON_RELEASE_TIME         (150U)
#define BUTTON_LONG_TIME            (800U)

extern void HandleButtonEvent(uint8_t evt, uint8_t btn);

#endif /* __BUTTON_CONFIG__ */
