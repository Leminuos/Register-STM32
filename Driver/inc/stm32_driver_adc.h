#ifndef __STM32_DRIVER_ADC__
#define __STM32_DRIVER_ADC__

#include "stm32f103.h"

#define ADC_PRESCALER_2     0x00
#define ADC_PRESCALER_4     0x01
#define ADC_PRESCALER_6     0x02
#define ADC_PRESCALER_8     0x03

extern void ADC_Prescaler(uint8_t pres);
extern void ADC_Init(void);
extern uint16_t ADC_ReadData(void);

#endif /* __STM32_DRIVER_ADC__ */
