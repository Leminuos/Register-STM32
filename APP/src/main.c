//#include "vscode.h"
#include "stm32f103.h"
#include <stdio.h>
void USART_Transmiter(USART_Typedef* USARTx, uint8_t data);

#if defined(__GNUC__)
int _write(int fd, char * ptr, int len) {
  
  return len;
}
#elif defined(__ICCARM__)
#include "LowLevelIOInterface.h" 
size_t __write(int handle, const unsigned char * buffer, size_t size) {
  
  return size;
}
#elif defined(__CC_ARM)
int fputc(int ch, FILE * f) {
  USART_Transmiter(USART1, ch);
  return ch;
}
#endif

void init(void);
void loop(void);
void TIM2_Init(void);
void setupHardware(void);

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

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR.BITS.PR1 == 1)
    {
        GPIOA->ODR.BITS.ODR7 = !GPIOA->ODR.BITS.ODR7;
        EXTI->PR.BITS.PR1 = 0;
        NVIC_ClearPendingIRQ(EXTI1_IRQn);
    }
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

void SysTick_Handler(void)
{
    
}

void TIM2_IRQHandler(void)
{
    if (TIM2->DIER.BITS.UIE && TIM2->SR.BITS.UIF)
    {
        //GPIOB->ODR.BITS.ODR0 = !GPIOB->ODR.BITS.ODR0;
        Led.Index = (Led.Index + 1) % 5;
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

void readMCO(void)
{
    // MCO is System Clock
    RCC->CFGR.BITS.MCO = 0x04;
    
    /* Cau hinh clock port A */
    RCC->APB2ENR.BITS.IOPAEN = 1;
    
    /* Cau hinh MCO */
    GPIOA->CRH.BITS.MODE8 = 0X03;
    GPIOA->CRH.BITS.CNF8 = 0x02;
}

void SystickConfig(void)
{
    /* Cau hinh systick */
    SysTick->VAL = 719UL;
    SysTick->LOAD = 719UL;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
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

void ButtonConfig(void)
{
    /* Cau hinh clock port A */
    RCC->APB2ENR.BITS.IOPAEN = 1;
    
    /* Cau hinh input GPIOA pin 1 */
    GPIOA->CRL.BITS.MODE1 = 0x00;
    GPIOA->CRL.BITS.CNF1 = 0X02;
    GPIOA->ODR.BITS.ODR1 = 1;
}

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
    RCC->CFGR.BITS.PPRE1 = 0X04;
    
    // APB2 prescaler
    RCC->CFGR.BITS.PPRE2 = 0X00;
    
    // Enable PLL
    RCC->CR.BITS.PLLON = 0X01;
    
    // Wait till PLL is ready
    while (RCC->CR.BITS.PLLRDY == 0);
    
    // Select PLL to System Clock
    RCC->CFGR.BITS.SW = 0x02;
    
    // Wait till System Clock is PLL Clock
    while (RCC->CFGR.BITS.SWS != 0x02);
}

void TIM2_Init(void)
{
    // Enable clock
    RCC->APB1ENR.BITS.TIM2EN = 0x01;
    
    // Config timer
    TIM2->ARR.REG = 1000UL;     // 1s => Update Event
    TIM2->CNT.REG = 0;
    TIM2->PSC.REG = 36000UL;    // 1ms => Counter
    TIM2->DIER.BITS.UIE = 0x01;
    TIM2->CR1.BITS.CEN = 0X01;
    
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

void TraceInit(void)
{
    // Config clock
    RCC->APB2ENR.BITS.IOPAEN = 0x01;
    RCC->APB2ENR.BITS.USART1EN = 0x01;
    
    // Config PA9 - Transmitter
    GPIOA->CRH.BITS.CNF9 = 0x02;
    GPIOA->CRH.BITS.MODE9 = 0x02;
    
    // Config PA10 - Receiver
    GPIOA->CRH.BITS.CNF10 = 0x02;
    GPIOA->CRH.BITS.MODE10 = 0x00;
    
    // Baud rate = 9600 => USARTDIV: 468.75
    USART1->BRR.BITS.Fraction = 0x0C;
    USART1->BRR.BITS.Mantissa = 0x1D4;
    
    // USART enable
    USART1->CR1.BITS.UE = 0x01;
    
    // Transmitter enable
    USART1->CR1.BITS.TE = 0x01;
    
    // Receiver Interrupt enable
    USART1->CR1.BITS.RXNEIE = 0x01;
    
    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 0X01);
    
    // Receiver enable
    USART1->CR1.BITS.RE = 0x01;
}

typedef struct
{
    uint8_t size;
    uint8_t buff[30];
} USARTPack;

USARTPack gUSART1Pack;
USARTPack gUSART2Pack;

void USART1_IRQHandler(void)
{
    if (USART1->CR1.BITS.RXNEIE && USART1->SR.BITS.RXNE)
    {
        if (gUSART1Pack.size < 30)
        {
            gUSART1Pack.buff[gUSART1Pack.size] = (uint8_t) USART1->DR.REG;
            ++gUSART1Pack.size;
        }
        
        USART1->SR.BITS.RXNE = 0;
    }
}

void USART2_IRQHandler(void)
{
    if (USART2->CR1.BITS.RXNEIE && USART2->SR.BITS.RXNE)
    {
        if (gUSART2Pack.size < 30)
        {
            gUSART2Pack.buff[gUSART2Pack.size] = (uint8_t) USART2->DR.REG;
            ++gUSART2Pack.size;
        }
        
        USART2->SR.BITS.RXNE = 0;
    }
}

void USARTInit(void)
{
    // Config clock
    RCC->APB2ENR.BITS.IOPAEN = 0x01;
    RCC->APB1ENR.BITS.USART2EN = 0x01;
    
    // Config PA2 - Transmitter
    GPIOA->CRL.BITS.CNF2 = 0x02;
    GPIOA->CRL.BITS.MODE2 = 0x02;
    
    // Config PA3 - Receiver
    GPIOA->CRL.BITS.CNF3 = 0x02;
    GPIOA->CRL.BITS.MODE3 = 0x00;
    
    // Baud rate = 9600 => USARTDIV: 234.375
    USART2->BRR.BITS.Fraction = 0x06;
    USART2->BRR.BITS.Mantissa = 0xEA;
    
    // USART enable
    USART2->CR1.BITS.UE = 0x01;
    
    // Transmitter enable
    USART2->CR1.BITS.TE = 0x01;
    
    // Receiver Interrupt enable
    USART2->CR1.BITS.RXNEIE = 0x01;
    
    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 0X01);
    
    // Receiver ennable
    USART2->CR1.BITS.RE = 0x01;
}

void USART_Transmiter(USART_Typedef* USARTx, uint8_t data)
{
    while (USARTx->SR.BITS.TC == 0);
    USARTx->DR.REG = data;
    USARTx->SR.BITS.TC = 0;
}

void TimerConfig(void)
{
    //TIM2_Init();
    //TIM3_Init();
}

void delayms(uint32_t ms)
{
    TIM2->CNT.REG = 0;
    while (TIM2->CNT.REG < ms);
}

void init(void)
{
    setupHardware();
    ButtonConfig();
    LEDRGBConfig();
    //SystickConfig();
    TimerConfig();
    TraceInit();
    USARTInit();
    EXTIConfig();
}

uint8_t debounce()
{
    static uint16_t state = 0;
    state = (state<<1) | GPIOA->IDR.BITS.IDR1 | 0xfe00;
    return (state == 0xff00);
}

uint32_t i;
char buff = 'A';

void loop(void)
{
    for (i = 0; i < 10; ++i) USART_Transmiter(USART1, buff + i);
    for (i = 0; i < 10000000; ++i);
}
