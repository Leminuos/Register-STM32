#ifndef __DRIVER_TIM__
#define __DRIVER_TIM__

#include "stm32f103.h"
#include "stm32_hal_util.h"

extern void delay(uint16_t mDelay);
extern uint16_t TIM_GetTimerCount(void);

#endif /* __DRIVER_TIM__ */
