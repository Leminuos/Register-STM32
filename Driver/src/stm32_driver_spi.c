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

uint8_t SPI_Transfer(SPI_Typedef* xSpi, uint8_t val)
{
   SPI_SendByte(xSpi, val);
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_BSY) == SET);
   return SPI_ReadByte(xSpi);
}

void SPI_WriteByte(SPI_Typedef* xSpi, uint8_t val)
{
   // Cờ TXE Set khi buffer trống => chờ buffer trống
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == RESET);

   // Ghi dữ liệu vào thanh ghi Data Register
   SPI_SendByte(xSpi, val);

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
   uint8_t data = 0;

   // Cờ TXE Set khi buffer trống => chờ buffer trống
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == RESET);

   // Gửi dummy data trước khi đọc dữ liệu
   SPI_SendByte(xSpi, 0xFF);
   
   // Cờ RNXE set khi buffer có dữ liệu => chờ buffer có dữ liệu
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_RXNE) == RESET);
   
   // Đọc dữ liệu từ thanh ghi Data Register
   data = SPI_ReadByte(xSpi);

   // Chờ cho đến khi bit BSY rảnh
   while (SPI_CHK_FLAG(xSpi, SPI_FLAG_BSY) == SET);

   // Clear cờ Overrun flag bằng cách đọc thanh ghi DR và SR
   uint8_t temp = xSpi->DR.REG;
   temp = xSpi->SR.REG;
   (void) temp;

   return data;
}
