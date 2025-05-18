#include "stm32_interrupt.h"
#include "stm32_hal_usb.h"

static uint32_t u32Tick;
static uint32_t u32Counter;

uint32_t GetCounterTick(void)
{
    return u32Tick;
}

void delay(uint32_t mDelay)
{
    uint32_t currTime = GetCounterTick();
    while (GetCounterTick() - currTime < mDelay);
}

void SysTick_Handler(void)
{
    ++u32Tick;
}

void TIM2_IRQHandler(void)
{
    if (TIM2->DIER.BITS.UIE && TIM2->SR.BITS.UIF)
    {
        ++u32Counter;

        if (u32Counter == 200)
        {
            u32Counter = 0;
            GPIOC->ODR.BITS.ODR13 = !GPIOC->ODR.BITS.ODR13;
        }

        TIM2->SR.BITS.UIF = 0;
        NVIC_ClearPendingIRQ(TIM2_IRQn);
    }
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    if (USB->ISTR.BITS.RESET != RESET)
    {
        USB_ResetCallBack();
    }
    
    if (USB->ISTR.BITS.CTR != RESET)
    {
        USB_TransactionCallBack();
    }
    
    if (USB->ISTR.BITS.ERR != RESET)
    {
        USB->ISTR.BITS.ERR = 0;
    }
    
    if (USB->ISTR.BITS.SOF != RESET)
    {
        USB->ISTR.BITS.SOF = 0;
    }
    
    if (USB->ISTR.BITS.ESOF != RESET)
    {
        USB->ISTR.BITS.ESOF = 0;
    }
    
    if (USB->ISTR.BITS.SUSP != RESET)
    {
        USB->ISTR.BITS.SUSP = 0;
    }
}
