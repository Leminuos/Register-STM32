#include "stm32f103.h"

void init(void);
void loop(void);


#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

struct {
    uint8_t     T;
    uint8_t     PWMVal;
    uint32_t    HexCodes[5]; 
    uint8_t     Index;
    uint8_t     Counter;
    uint32_t    Interval;
    uint32_t    Step;
    uint8_t     Linear;
} Led = {
    .T          = 0xFF,
    .PWMVal     = 2,
    .HexCodes   = {0x33FFFF, 0xCC66FF, 0x00EE00, 0x00FF00, 0xFFF68F},
    .Interval   = 200,
    .Index      = 0,
    .Step       = 0,
    .Linear     = 0,
    .Counter    = 0xFF
};

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR.BITS.PR1 == 1)
    {
        GPIOA->ODR.BITS.ODR5 = !GPIOA->ODR.BITS.ODR5;
        EXTI->PR.BITS.PR1 = 1;
        NVIC_ClearPendingIRQ(EXTI1_IRQn);
    }
}

void BlinkLedRGB(void)
{
    ++Led.Counter;
    
    if (Led.Counter == ((Led.HexCodes[Led.Index] >> 16) & 0xFF))   // Red
        GPIOA->ODR.BITS.ODR5 = 0;
    if (Led.Counter == ((Led.HexCodes[Led.Index] >> 8) & 0xFF))    // Green
        GPIOA->ODR.BITS.ODR6 = 0;
    if (Led.Counter == (Led.HexCodes[Led.Index] & 0xFF))           // Blue
        GPIOA->ODR.BITS.ODR4 = 0;
    
    if (Led.Counter == Led.T)
    {
        GPIOA->ODR.BITS.ODR4 = 1;
        GPIOA->ODR.BITS.ODR5 = 1;
        GPIOA->ODR.BITS.ODR6 = 1;
        Led.Counter = 0;
        ++Led.Step;
    }
    
    if (Led.Step == Led.Interval)
    {
        Led.Step = 0;
        Led.Index = (Led.Index + 1) % 5;
    }
}

void BlinkLedRGBv2(void)
{
    
}

void SysTick_Handler(void)
{
    BlinkLedRGB();
}

void init(void)
{
    /* Cau hinh clock port A */
    RCC->APB2ENR = 0x01 << 2;
    
    /* Cau hinh output GPIOA pin 6 - Green*/
    GPIOA->CRL.BITS.MODE6 = 0x03;
    GPIOA->CRL.BITS.CNF6 = 0x00;
    GPIOA->ODR.BITS.ODR6 = 0;
    
    /* Cau hinh output GPIOA pin 5 - Red */
    GPIOA->CRL.BITS.MODE5 = 0x03;
    GPIOA->CRL.BITS.CNF5 = 0x00;
    GPIOA->ODR.BITS.ODR5 = 0;
    
    /* Cau hinh output GPIOA pin 4 - Blue*/
    GPIOA->CRL.BITS.MODE4 = 0x03;
    GPIOA->CRL.BITS.CNF4 = 0x00;
    GPIOA->ODR.BITS.ODR4 = 0;

    /* Cau hinh input GPIOA pin 1 */
    GPIOA->CRL.BITS.MODE1 = 0x00;
    GPIOA->CRL.BITS.CNF1 = 0X02;
    GPIOA->ODR.BITS.ODR1 = 1;
    
    /* Cau hinh systick */
    SysTick->VAL = 99UL;
    SysTick->LOAD = 99UL;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
    
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

uint8_t debounce()
{
    static uint16_t state = 0;
    state = (state<<1) | GPIOA->IDR.BITS.IDR1 | 0xfe00;
    return (state == 0xff00);
}

void loop(void)
{
    
}
