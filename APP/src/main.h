#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <string.h>
#include "stm32f103.h"
#include "stm32_hal_util.h"
#include "bootloader.h"

void init(void);
void loop(void);
extern void setupHardware(void);
extern void TestLed(void);

#endif /* __MAIN_H__ */
