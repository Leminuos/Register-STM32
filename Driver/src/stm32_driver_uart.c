#include "stm32_driver_uart.h"
#include <stdio.h>

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

/* FILE is typedef in stdio.h. */
FILE __stdout;

int fputc(int ch, FILE *f)
{
  /* Your implementation of fputc(). */
  USART_TransmiterString(USART1, (uint8_t*) &ch);
  return ch;
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
    
    // Baud rate = 115000 => USARTDIV: 39.0625
    USART1->BRR.BITS.Fraction = 0x01;
    USART1->BRR.BITS.Mantissa = 0x27;
    
    // Transmitter enable
    USART1->CR1.BITS.TE = 0x01;
    
    // Receiver enable
    USART1->CR1.BITS.RE = 0x01;
    
    // Receiver Interrupt enable
    USART1->CR1.BITS.RXNEIE = 0x01;
    
    // USART enable
    USART1->CR1.BITS.UE = 0x01;
    
    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 0X01);
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
    GPIOA->CRL.BITS.MODE2 = 0x03;
    
    // Config PA3 - Receiver
    GPIOA->CRL.BITS.CNF3 = 0x02;
    GPIOA->CRL.BITS.MODE3 = 0x00;
    GPIOA->ODR.BITS.ODR3 = 0x01;
    
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
}

void USART_TransmiterString(USART_Typedef* USARTx, uint8_t* str)
{
    while (*str != 0U)
    {
        USART_Transmiter(USARTx, *str);
        str++;
    }
}
