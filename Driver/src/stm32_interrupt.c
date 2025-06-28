#include "stm32_interrupt.h"
#include "stm32_hal_usb.h"
#include "debug.h"

extern void ButtonProcess(void);

static uint32_t u32Tick;

uint32_t GetCounterTick(void)
{
    return u32Tick;
}

void delay(uint16_t mDelay)
{
    uint32_t currTime = GetCounterTick();
    while (GetCounterTick() - currTime < mDelay);
}

void SysTick_Handler(void)
{
    ++u32Tick;
    // ButtonProcess();
}

void prvGetRegistersFromStack(uint32_t* pulFaultStackAddress);

__asm void HardFault_Handler(void)
{
    IMPORT prvGetRegistersFromStack
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp
    ldr r1, [r0, #24]
    ldr r2, =prvGetRegistersFromStack
    bx r2
}

__asm void MemManage_Handler(void)
{
    IMPORT prvGetRegistersFromStack
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp
    ldr r1, [r0, #24]
    ldr r2, =prvGetRegistersFromStack
    bx r2
}

__asm void BusFault_Handler(void)
{
    IMPORT prvGetRegistersFromStack
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp
    ldr r1, [r0, #24]
    ldr r2, =prvGetRegistersFromStack
    bx r2
}

__asm void UsageFault_Handler(void)
{
    IMPORT prvGetRegistersFromStack
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp
    ldr r1, [r0, #24]
    ldr r2, =prvGetRegistersFromStack
    bx r2
}

void prvGetRegistersFromStack(uint32_t* pulFaultStackAddress)
{
    volatile uint32_t r0  = 0;
    volatile uint32_t r1  = 0;
    volatile uint32_t r2  = 0;
    volatile uint32_t r3  = 0;
    volatile uint32_t r12 = 0;
    volatile uint32_t lr  = 0;
    volatile uint32_t pc  = 0;
    volatile uint32_t psr = 0;

    r0  = pulFaultStackAddress[0];
    r1  = pulFaultStackAddress[1];
    r2  = pulFaultStackAddress[2];
    r3  = pulFaultStackAddress[3];
    r12 = pulFaultStackAddress[4];
    lr  = pulFaultStackAddress[5];
    pc  = pulFaultStackAddress[6];
    psr = pulFaultStackAddress[7];

    debug_print("r0  : %08X\r\n", r0);
    debug_print("r1  : %08X\r\n", r1);
    debug_print("r2  : %08X\r\n", r2);
    debug_print("r3  : %08X\r\n", r3);
    debug_print("r12 : %08X\r\n", r12);
    debug_print("lr  : %08X\r\n", lr);
    debug_print("pc  : %08X\r\n", pc);
    debug_print("psr : %08X\r\n", psr);

    // Soft reset
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
