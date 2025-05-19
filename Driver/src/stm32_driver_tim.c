#include "stm32_driver_tim.h"

void TIM2_IRQHandler(void)
{
    if (TIM2->DIER.BITS.UIE && TIM2->SR.BITS.UIF)
    {
        TIM2->SR.BITS.UIF = 0;
        NVIC_ClearPendingIRQ(TIM2_IRQn);
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->DIER.BITS.UIE && TIM3->SR.BITS.UIF)
    {
        //GPIOB->ODR.BITS.ODR0 = !GPIOB->ODR.BITS.ODR0;
        TIM3->SR.BITS.UIF = 0;
        NVIC_ClearPendingIRQ(TIM3_IRQn);
    }
}

void TIM2_Init(void)
{
    // Enable clock
    RCC->APB1ENR.BITS.TIM2EN = 0x01;
    
    // Config timer
    TIM2->ARR.REG = 1000UL;     // 1s => Update Event
    TIM2->CNT.REG = 0;
    TIM2->PSC.REG = 71UL;    // 1us => Counter
    TIM2->DIER.BITS.UIE = 0x01;
    TIM2->CR1.BITS.CEN = 0x01;

    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0X01);
}

void TIM3_Init(void)
{
    // Enable clock
    RCC->APB1ENR.BITS.TIM3EN = 0x01;
    
    // Config timer: 1clock = 36MHz
    TIM3->ARR.REG = 1000UL;     // 1ms => Update Event
    TIM3->CNT.REG = 0UL;
    TIM3->PSC.REG = 36UL;       // 1us => Counter
    
    // Alternate Function
    GPIOB->CRL.BITS.MODE0 = 0x03;
    GPIOB->CRL.BITS.CNF0 = 0x02;
    
    // PWM Led Blue
    TIM3->CCER.BITS.CC1E = 0x01;
    TIM3->CCER.BITS.CC1P = 0X00;
    TIM3->CCMR1.BITS_O.OC1M = 0x06;
    TIM3->CCR1.REG = 800UL;
    
    // PWM Led Red
    TIM3->CCER.BITS.CC2E = 0x01;
    TIM3->CCER.BITS.CC2P = 0X00;
    TIM3->CCMR1.BITS_O.OC2M = 0x06;
    TIM3->CCR2.REG = 400UL;
    
    // PWM Led Green
    TIM3->CCER.BITS.CC3E = 0x01;
    TIM3->CCER.BITS.CC3P = 0X00;
    TIM3->CCMR2.BITS_O.OC3M = 0x06;
    TIM3->CCR3.REG = 200UL;
    
    // Enable Interrupt
    //TIM3->DIER.BITS.UIE = 0x01;
    
    /* Cau hinh ngat NVIC */
    // NVIC_EnableIRQ(TIM3_IRQn);
    // NVIC_SetPriority(TIM3_IRQn, 0X01);
    
    // Enable Timer
    TIM3->CR1.BITS.CEN = 0x01;
}

void TimerConfig(void)
{
    TIM2_Init();
}
