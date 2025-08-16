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

   /* Chọn tốc độ baudrate cho SPI */
   SPI_SetupBaudrate(xSpi, FPCLK16);

   /* Chọn Clock polarity và Clock Phase */
   SPI_SetupClockMode(xSpi, MODE0);

   /* Chon data frame format và LSB First */
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

void SPI_Transmit(SPI_Typedef* xSpi, const uint8_t *TxBuffer, uint16_t TxCount)
{
   if (xSpi->CR1.BITS.SPE == RESET)
   {
      /* Enable SPI peripheral */
      SPI_PERIPHERAL_ENABLE(xSpi);
   }

   if (TxCount == 0x01U)
   {
      SPI_SendByte(xSpi, *TxBuffer);
      TxBuffer++;
      TxCount--;
   }

   while (TxCount > 0U)
   {
      // Cờ TXE Set khi buffer trống => chờ buffer trống
      if (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == SET)
      {
         // Ghi dữ liệu vào thanh ghi Data Register
         SPI_SendByte(xSpi, *TxBuffer);
         TxBuffer++;
         TxCount--;
      }
   }

   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == RESET);

   // Kiểm tra transaction đã hoàn thành hay chưa
   // Cờ BSY set khi SPI đang bận truyền/nhận dữ liệu => Chờ đến khi SPI rảnh
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_BSY) == SET);

   // Clear cờ Overrun flag bằng cách đọc thanh ghi DR và SR
   uint8_t temp = xSpi->DR.REG;
   temp = xSpi->SR.REG;
   (void) temp;
}

void SPI_TransmitReceive(SPI_Typedef* xSpi, const uint8_t *TxBuffer, uint8_t* RxBuffer, uint16_t Size)
{
   uint16_t RxCount = Size;
   uint16_t TxCount = Size;
   uint32_t txallowed = 1U;

   if (xSpi->CR1.BITS.SPE == RESET)
   {
      /* Enable SPI peripheral */
      SPI_PERIPHERAL_ENABLE(xSpi);
   }
   
   if (TxCount == 0x01U)
   {
      SPI_SendByte(xSpi, *TxBuffer);
      TxBuffer++;
      TxCount--;
   }

   while (TxCount > 0U || RxCount > 0U)
   {
      // Cờ TXE Set khi buffer trống => chờ buffer trống
      if (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == SET && (TxCount > 0U) && (txallowed == 1U))
      {
         // Ghi dữ liệu vào thanh ghi Data Register
         SPI_SendByte(xSpi, *TxBuffer);
         TxBuffer++;
         TxCount--;

         /* Next Data is a reception (Rx). Tx not allowed */
         txallowed = 0U;
      }

      if (SPI_CHK_FLAG(xSpi, SPI_FLAG_RXNE) == SET && (RxCount > 0U))
      {
         *RxBuffer = SPI_ReadByte(xSpi);
         RxBuffer++;
         RxCount--;
         /* Next Data is a Transmission (Tx). Tx is allowed */
         txallowed = 1U;
      }
   }

   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == RESET);

   // Kiểm tra transaction đã hoàn thành hay chưa
   // Cờ BSY set khi SPI đang bận truyền/nhận dữ liệu => Chờ đến khi SPI rảnh
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_BSY) == SET);

   // Clear cờ Overrun flag bằng cách đọc thanh ghi DR và SR
   uint8_t temp = xSpi->DR.REG;
   temp = xSpi->SR.REG;
   (void) temp;
}

uint8_t SPI_ReceiveByte(SPI_Typedef* xSpi)
{
   uint8_t dummy = 0xFF;
   uint8_t data = 0;

   SPI_TransmitReceive(xSpi, &dummy, &data, 1);

   return data;
}

void SPI_WriteByte(SPI_Typedef* xSpi, uint8_t val)
{
   SPI_Transmit(xSpi, &val, 1);
}

uint8_t SPI_Transfer(SPI_Typedef* xSpi, uint8_t val)
{
   uint8_t retdata = 0;

   SPI_TransmitReceive(xSpi, &val, &retdata, 1);

   return retdata;
}
