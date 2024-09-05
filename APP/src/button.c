#include "stm32f103.h"

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR.BITS.PR1 == 1)
    {
        GPIOA->ODR.BITS.ODR7 = !GPIOA->ODR.BITS.ODR7;
        EXTI->PR.BITS.PR1 = 0;
        NVIC_ClearPendingIRQ(EXTI1_IRQn);
    }
}

void EXTIConfig(void)
{
    /* Cau hinh ngat EXTI1 */
    AFIO->EXTICR1.BITS.EXTI1 = 0x00;
    EXTI->IMR.BITS.MR1 = 0x01;
    EXTI->FTSR.BITS.TR1 = 0x01;
    
    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_SetPriority(EXTI1_IRQn, 0X01);
    
    /* Bat ngat toan cuc */
    __ASM("CPSIE I");
}

void ButtonConfig(void)
{
    /* Cau hinh clock port A */
    RCC->APB2ENR.BITS.IOPAEN = 1;
    
    /* Cau hinh input GPIOA pin 1 */
    GPIOA->CRL.BITS.MODE1 = 0x00;
    GPIOA->CRL.BITS.CNF1 = 0X02;
    GPIOA->ODR.BITS.ODR1 = 1;
}
