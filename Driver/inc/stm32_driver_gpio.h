#ifndef __DRIVER_GPIO__
#define __DRIVER_GPIO__

#include <stdint.h>
#include "stm32_hal_gpio.h"
#include "stm32_hal_util.h"

void GPIO_SetBit(GPIO_Typedef* Port, uint16_t Pin);
void GPIO_ResetBit(GPIO_Typedef* Port, uint16_t Pin);
uint8_t GPIO_ReadInputDataBit(GPIO_Typedef* Port, uint16_t Pin);
void GPIO_Init(GPIO_Typedef* Port, GPIO_InitTypeDef* GPIO_InitStruct);

#endif /* __DRIVER_GPIO__ */
