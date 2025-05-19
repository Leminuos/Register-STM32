#include "stm32_interrupt.h"
#include "main.h"

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
