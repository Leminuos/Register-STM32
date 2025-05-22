#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f103.h"
#include "stm32_driver_gpio.h"
#include "stm32_driver_uart.h"
#include "stm32_driver_spi.h"
#include "stm32_driver_tim.h"
#include "stm32_hal_usb.h"
#include "stm32_hal_util.h"

void init(void);
void loop(void);
void SystickConfig(uint32_t u32Reload);
extern void TraceInit(void);
extern void delay(uint16_t mDelay);
extern void setupHardware(void);
extern void TestLed(void);

#endif /* __MAIN_H__ */

