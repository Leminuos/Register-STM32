#include "stm32f103.h"
#include "stm32_hal_util.h"

struct {
    uint8_t     T;
    uint32_t    HexCodes[5]; 
    uint8_t     Index;
    uint8_t     Counter;
    uint8_t     Linear;
} Led = {
    .T          = 0xFF,
    .HexCodes   = {0x33FFFF, 0xCC66FF, 0x00EE00, 0x00FF00, 0xFFF68F},
    .Index      = 0,
    .Linear     = 0,
    .Counter    = 0xFF
};

void TestLed(void)
{
    RCC->APB2ENR.BITS.IOPAEN = SET;
    RCC->APB2ENR.BITS.IOPBEN = SET;
    RCC->APB2ENR.BITS.IOPCEN = SET;

    GPIOA->CRL.BITS.MODE7 = 0x03;
    GPIOA->CRL.BITS.CNF7 = 0x00;
    GPIOA->ODR.BITS.ODR7 = 0;

    GPIOA->CRL.BITS.MODE6 = 0x03;
    GPIOA->CRL.BITS.CNF6 = 0x00;
    GPIOA->ODR.BITS.ODR6 = 0;

    GPIOB->CRL.BITS.MODE0 = 0x03;
    GPIOB->CRL.BITS.CNF0 = 0x00;
    GPIOB->ODR.BITS.ODR0 = 0;
    
    GPIOC->CRH.BITS.MODE13 = 0x03;
    GPIOC->CRH.BITS.CNF13 = 0x00;
    GPIOC->ODR.BITS.ODR13 = 1;
}

void LEDRGBConfig(void)
{
    /* Cau hinh clock port A */
    RCC->APB2ENR.BITS.IOPAEN = 1;
    
    /* Cau hinh clock port B */
    RCC->APB2ENR.BITS.IOPBEN = 1;
    
    /* Cau hinh Alternate Function GPIOB pin 0 - Green*/
    GPIOB->CRL.BITS.MODE0 = 0x03;
    GPIOB->CRL.BITS.CNF0 = 0x02;
    GPIOB->ODR.BITS.ODR0 = 0;
    
    /* Cau hinh Alternate Function GPIOA pin 7 - Red */
    GPIOA->CRL.BITS.MODE7 = 0x03;
    GPIOA->CRL.BITS.CNF7 = 0x02;
    GPIOA->ODR.BITS.ODR7 = 0;
    
    /* Cau hinh Alternate Function GPIOA pin 6 - Blue*/
    GPIOA->CRL.BITS.MODE6 = 0x03;
    GPIOA->CRL.BITS.CNF6 = 0x02;
    GPIOA->ODR.BITS.ODR6 = 0;
}

void BlinkLedRGB(void)
{
    ++Led.Counter;
    
    if (Led.Counter == ((Led.HexCodes[Led.Index] >> 16) & 0xFF))   // Red
        GPIOA->ODR.BITS.ODR7 = 0;
    if (Led.Counter == ((Led.HexCodes[Led.Index] >> 8) & 0xFF))    // Green
        GPIOB->ODR.BITS.ODR0 = 0;
    if (Led.Counter == (Led.HexCodes[Led.Index] & 0xFF))           // Blue
        GPIOA->ODR.BITS.ODR6 = 0;
    
    if (Led.Counter == Led.T)
    {
        GPIOA->ODR.BITS.ODR6 = 1;
        GPIOA->ODR.BITS.ODR7 = 1;
        GPIOB->ODR.BITS.ODR0 = 1;
        Led.Counter = 0;
    }
}
