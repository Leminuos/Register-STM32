#include "stm32_interrupt.h"
#include "main.h"

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
