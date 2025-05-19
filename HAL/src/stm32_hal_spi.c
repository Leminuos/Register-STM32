#include "stm32_hal_spi.h"

/* Chọn tốc độ baud rate cho SPI
   SPI Clock = APB Clock / (2^(BR + 1)) */
void SPI_SetupBaudrate(SPI_Typedef* xSpi, SPI_Baudrate Baudrate)
{
   xSpi->CR1.BITS.BR = (uint8_t) Baudrate;
}

void SPI_CRC_Polynomial(SPI_Typedef* xSpi, uint16_t val)
{
   xSpi->CRCPR.BITS.CRCPOLY = val;
}

/* Software NSS management - SSM = 1
   Chọn slave thông qua bit SSI và không cần sử dụng chân Slave Select - SS
   => Nếu không sử dụng chân SS thì ta có thể mô phỏng nó bằng phần mềm khi set bit SSM lên 1. 
   Chân NSS nên được connect tới VDD để tránh một số lỗi

   Hardware NSS management - SSM = 0: Phần cứng sẽ điều khiển chân SS.

   SSI (Internal Slave Select): Bit này sẽ mô phỏng tín hiệu Slave Select khi SSM được set.
   - Đối với device ở chế độ slave:
    + SSI = 0: Giả lập NSS ở mức thấp -> SPI hoạt động ở chế độ Slave.
    + SSI = 1: Giả lập NSS ở mức cao -> SPI không hoạt động ở chế độ Slave.
   - Đối với device ở chế độ master:
    + Khi chân NSS được sử dụng làm chân đầu vào -> được sử dụng cho mục đích multimaster.

   SSOE (SS output enable): 
   + SSOE = 1: Chỉ được sử dụng ở chế độ master, chân SS được tự động kéo xuống mức thấp
   khi master bắt đầu truyền dữ liệu và được giữ ở mức thấp cho đến khi truyền kết thúc.
   + SSOE = 0: Cấu hình này sẽ cho phép các device ở chế độ multimaster, còn đối với chế độ
   slave thì slave được chọn khi chân SS ở mức thấp và không được chọn khi chân SS ở mức cao.
*/
void SPI_SetupNSS(SPI_Typedef* xSpi, SPI_NSS Config)
{
   xSpi->CR1.BITS.SSM = (uint8_t) Config;
}

/* 
   CPOL - Clock polarity
   CPOL cho biết trạng thái idle của clock khi không có data được truyền
   + CPOL = 0 => CLK idle = 0
   + CPOL = 1 => CLK idle = 1

   CPHA - Clock phase:
   CPHA cho biết cạnh thứ nhất hay thứ hai của clock để latch data.
   + CPHA = 0 => second edge
   + CPHA = 1 => first egde

   Trước khi change bit CPOL/CPHA thì phải disable bit SPE

   https://www.totalphase.com/media/blog/2013/08/CheetahSPIClockPhases.jpg
*/
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

/*
   LSBFIRST (LSB First): Chọn thứ tự truyền bit
   + LSBFIRST = 0: MSB trước.
   + LSBFIRST = 1: LSB trước.

   DFF (Data Frame Format): Chọn độ dài dữ liệu, chế độ này ảnh hưởng đến cách đọc/ghi dữ liệu trong thanh ghi.
   + DFF = 0: Dữ liệu 8-bit.
   + DFF = 1: Dữ liệu 16-bit.
*/
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

/* Chế độ bidirectional: Ở chế độ này sẽ chỉ dùng một dây dữ liệu thay vì 2 dây,
   tức là, MOSI và MISO sẽ hợp nhất thành một dây duy nhất.
   -> Gửi hoặc nhận tại một thời điểm -> Half-Duplex
   Chân dữ liệu sẽ là MOSI đối với master và MISO đối với slave.
   Hướng truyền sẽ được select thông qua bit BIDIOE với:
   + Chân Data Line được cấu hình làm đầu ra
   + Chân Data Line được cấu hình làm đầu vào

   Chế độ Unidirectional: Đây là chế độ mặc định của SPI, trong đó có hai đường
   dữ liệu riêng biệt là MISO và MOSI.
*/
void SPI_SetDirection(SPI_Typedef* xSpi, SPI_Direction dir)
{
    return;
}

void SPI_SendByte(SPI_Typedef* xSpi, uint8_t data)
{
   xSpi->DR.BITS.DR = data;
}

uint8_t SPI_ReadByte(SPI_Typedef* xSpi)
{
   /* Read the SPI_DR register to clear the RXNE bit */
   return xSpi->DR.BITS.DR;
}
