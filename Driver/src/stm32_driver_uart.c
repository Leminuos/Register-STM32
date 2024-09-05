#include "stm32_driver_uart.h"
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
