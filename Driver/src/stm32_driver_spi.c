#include "stm32_driver_spi.h"

void SPI_Init(SPI_Typedef* xSpi)
{
   if (xSpi == SPI1)
   {
      RCC->APB2ENR.BITS.IOPAEN = SET;

      GPIOA->CRL.BITS.CNF5 = 0x02;
      GPIOA->CRL.BITS.MODE5 = 0x03;

      GPIOA->CRL.BITS.CNF6 = 0x01;
      GPIOA->CRL.BITS.MODE6 = 0x00;

      GPIOA->CRL.BITS.CNF7 = 0x02;
      GPIOA->CRL.BITS.MODE7 = 0x03;

      RCC->APB2ENR.BITS.SPI1EN = SET;
       
      NVIC_SetPriority(SPI1_IRQn, 0x00);
      NVIC_EnableIRQ(SPI1_IRQn);
   }
   else if (xSpi == SPI2)
   {
      RCC->APB2ENR.BITS.IOPBEN = SET;

      GPIOB->CRH.BITS.CNF12 = 0x00;
      GPIOB->CRH.BITS.MODE12 = 0x03;
      GPIOB->ODR.BITS.ODR12 = 0x01;

      GPIOB->CRH.BITS.CNF13 = 0x02;
      GPIOB->CRH.BITS.MODE13 = 0x03;

      GPIOB->CRH.BITS.CNF14 = 0x01;
      GPIOB->CRH.BITS.MODE14 = 0x00;

      GPIOB->CRH.BITS.CNF15 = 0x02;
      GPIOB->CRH.BITS.MODE15 = 0x03;
      
      RCC->APB1ENR.BITS.SPI2EN = SET;
       
      NVIC_SetPriority(SPI2_IRQn, 0x00);
      NVIC_EnableIRQ(SPI2_IRQn);
   }
   else
   {
      // Do something
   }

   /* Disable SPI */
   SPI_PERIPHERAL_DISABLE(xSpi);

   /* Select the BR[2:0] bits to define the serial clock baud rate */
   SPI_SetupBaudrate(xSpi, FPCLK16);

   /* Select the CPOL and CPHA bits */
   SPI_SetupClockMode(xSpi, MODE0);

   /* Set the DFF bit to define 8- or 16-bit data frame format */
   /* Configure the LSBFIRST bit in the SPI_CR1 register to define the frame format */
   SPI_SetupFrameFormat(xSpi, DATA8BIT_MSB_FIRST);

   /* Configure NSS pin */
   SPI_SetupNSS(xSpi, NSS_SOFT);

   /* Active SPI mode */
   SPI_ACTIVE(xSpi);
   
   /* Write to SPIx CRCPOLY */
   SPI_CRC_Polynomial(xSpi, 0x0007);

   /* Set MSTR and SPE bits */
   SPI_MASTER_SELECTION(xSpi);
   SPI_PERIPHERAL_ENABLE(xSpi);
}

uint8_t SPI_Transfer(SPI_Typedef* xSpi, uint8_t val)
{
   SPI_SendByte(xSpi, val);
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_BSY) == SET);
   return SPI_ReadByte(xSpi);
}
