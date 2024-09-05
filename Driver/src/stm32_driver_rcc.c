#include "stm32_driver_rcc.h"

void setupHardware(void)
{
    // Enable HSE
    RCC->CR.BITS.HSEON = 0x01;
    
    // Wait till HSE is ready
    while (RCC->CR.BITS.HSERDY == 0);
    
    /* 2 wait states required on the flash. */
	*((unsigned long * ) 0x40022000 ) = 0x02;
    
    // HSE clock not divided
    RCC->CFGR.BITS.PLLXTPRE = 0x00;
    
    // Select HSE Clock to PLL Clock
    RCC->CFGR.BITS.PLLSRC = 0x01;
    
    // PLLMUX: PLL * 9 = 72MHz
    RCC->CFGR.BITS.PLLMUL = 0x07;
    
    // AHB prescaler
    RCC->CFGR.BITS.HPRE = 0x00;
    
    // APB1 prescaler: AHB / 2
    RCC->CFGR.BITS.PPRE1 = 0x04;
    
    // APB2 prescaler
    RCC->CFGR.BITS.PPRE2 = 0x00;
    
    // Enable PLL
    RCC->CR.BITS.PLLON = 0x01;
    
    // Wait till PLL is ready
    while (RCC->CR.BITS.PLLRDY == 0);
    
    // Select PLL to System Clock
    RCC->CFGR.BITS.SW = 0x02;
    
    // Wait till System Clock is PLL Clock
    while (RCC->CFGR.BITS.SWS != 0x02);
    
    RCC->APB2ENR.BITS.AFIOEN = SET;
}
