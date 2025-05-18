#include "stm32_driver_tim.h"

void TIM2_Init(void)
{
    // Enable clock
    RCC->APB1ENR.BITS.TIM2EN = 0x01;
    
    // Config timer
    TIM2->ARR.REG = 1000UL;     // 1ms => Update Event
    TIM2->CNT.REG = 0;
    TIM2->PSC.REG = 71UL;       // 1us => Counter

    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0x01);
}

void TimerConfig(void)
{
    TIM2_Init();
}
