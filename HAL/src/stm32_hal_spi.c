#include "stm32_hal_spi.h"

void SPI_SetupBaudrate(SPI_Typedef* xSpi, SPI_Baudrate Baudrate)
{
   xSpi->CR1.BITS.BR = (uint8_t) Baudrate;
}

void SPI_CRC_Polynomial(SPI_Typedef* xSpi, uint16_t val)
{
   xSpi->CRCPR.BITS.CRCPOLY = val;
}

void SPI_SendByte(SPI_Typedef* xSpi, uint8_t data)
{
   xSpi->DR.BITS.DR = data;
   /* The TXE flag is set on the transfer of data from the Tx Buffer to the shift register
   and an interrupt is generated if the TXEIE bit in the SPI_CR2 register is set. */
   //while (SPI_CHK_FLAG(xSpi, SPI_FLAG_TXE) == SET);
}

uint8_t SPI_ReadByte(SPI_Typedef* xSpi)
{
   /* The RXNE flag is set on the transfer of data from the shift register to the Rx Buffer
   and an interrupt is generated if the RXNEIE bit in the SPI_CR2 register is set. */
   //while (SPI_CHK_FLAG(xSpi, SPI_FLAG_RXNE) == SET);
   
   /* Read the SPI_DR register to clear the RXNE bit */
   return xSpi->DR.BITS.DR;
}

//====================================================================
/* Software NSS management - SSM = 1
   Chọn slave select thông qua bit SSI và không cần sử dụng chân NSS
   Only single master - single slave
   Chân NSS nên được connect tới VDD để tránh một số lỗi */
//====================================================================

//====================================================================
/* Hardware NSS management - SSM = 0
   Sử dụng các chân IO khác của master để select slave.

   + NSS output enabled (SSM = 0, SSOE = 1): Device ở chế độ
   master mode. NSS ở mức thấp khi master bắt đầu giao dịch và
   giữ cho đến khi SPI disable.

   + NSS output disabled (SSM = 0, SSOE = 0): device ở chế độ
   slave mode hoặc multi-master. Slave được select khi chân NSS
   ở mức thấp và deselect khi chân NSS ở mức cao. */
//====================================================================

void SPI_SetupNSS(SPI_Typedef* xSpi, SPI_NSS Config)
{
   xSpi->CR1.BITS.SSM = (uint8_t) Config;
}


//====================================================================
/* CPOL - Clock polarity
   CPOL cho biết trạng thái idle của clock khi không có data được truyền
   + CPOL = 0 => CLK idle = 0
   + CPOL = 1 => CLK idle = 1 */
/* CPHA - Clock phase
   CPHA cho biết edge thứ nhất hay thứ hai của clock để latch data.
   + CPHA = 0 => second edge
   + CPHA = 1 => first egde */
// Trước khi change bit CPOL/CPHA thì phải disable bit SPE
//====================================================================

void SPI_SetupClockMode(SPI_Typedef* xSpi, SPI_ClockMode Mode)
{
   if (Mode & BIT0 == BIT0)
      xSpi->CR1.BITS.CPHA = SET;
   else
      xSpi->CR1.BITS.CPHA = RESET;

   if (Mode & BIT1 == BIT1)
      xSpi->CR1.BITS.CPOL = SET;
   else
      xSpi->CR1.BITS.CPOL = RESET;
}

void SPI_SetupFrameFormat(SPI_Typedef* xSpi, SPI_FrameFormat Format)
{
   if (Format & BIT7 == BIT7)
      xSpi->CR1.BITS.LSBFIRST = SET;
   else
      xSpi->CR1.BITS.LSBFIRST = RESET;

   if (Format & BIT11 == BIT11)
      xSpi->CR1.BITS.DFF = SET;
   else
      xSpi->CR1.BITS.DFF = RESET;
}

/* When a master is communicating with SPI slaves which need to be de-selected between
transmissions, the NSS pin must be configured as GPIO or another GPIO must be used and
toggled by software. */

