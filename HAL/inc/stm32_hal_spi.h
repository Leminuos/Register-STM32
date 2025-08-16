#ifndef __HAL_SPI__
#define __HAL_SPI__

#include <stdint.h>
#include "stm32f103.h"
#include "stm32_hal_util.h"

typedef enum {
   NSS_HARDWARE = 0x00,
   NSS_SOFT = 0x01,
} SPI_NSS;

/* If the SPI peripheral, that microcontroller is getting 50MHz of a peripheral clock, then the
   maximum speed of a peripheral clock is 25Mbps. Because the maximum speed of the peripheral clock is
   divided by 2 (fPCLK/2). Hence, SPI works over a shorter distance but at a higher data rate or higher speed. */

typedef enum {
   FPCLK2   = 0x00,
   FPCLK4   = 0x01,
   FPCLK8   = 0x02,
   FPCLK16  = 0x03,
   FPCLK32  = 0x04,
   FPCLK64  = 0x05,
   FPCLK128 = 0x06,
   FPCLK256 = 0x07,
} SPI_Baudrate;

typedef enum {
   MODE0    = 0x00,
   MODE1    = 0x01,
   MODE2    = 0x02,
   MODE3    = 0x03,
} SPI_ClockMode;

typedef enum {
   DATA8BIT_MSB_FIRST   = 0x0000,
   DATA8BIT_LSB_FIRST   = 0x0080,
   DATA16BIT_MSB_FIRST  = 0x0800,
   DATA16BIT_LSB_FIRST  = 0x0880,
} SPI_FrameFormat;

typedef enum {
   SPI_DIRECTION_FULLDUPLEX                  ,     // BIDIMODE=0 and RXONLY=0
   SPI_DIRECTION_RECEIVE_ONLY                ,     // BIDIMODE=0 and RXONLY=1
   SPI_DIRECTION_HALFDUPLEX_RECEIVE_ONLY     ,     // BIDIMODE=1 and BIDIOE=0
   SPI_DIRECTION_HALFDUPLEX_TRANSMIT_ONLY    ,     // BIDIMODE=1 and BIDIOE=1
} SPI_Direction;

#define SPI_FLAG_RXNE                        BIT0
#define SPI_FLAG_TXE                         BIT1
#define SPI_FLAG_CHSIDE                      BIT2
#define SPI_FLAG_UDR                         BIT3
#define SPI_FLAG_CRCERR                      BIT4
#define SPI_FLAG_MODF                        BIT5
#define SPI_FLAG_OVR                         BIT6
#define SPI_FLAG_BSY                         BIT7

#define SPI_CHK_FLAG(__INSTANCE__, __FLAG__) ((((__INSTANCE__)->SR.REG & (__FLAG__)) == (__FLAG__)) ? SET : RESET)

/* SPE (SPI Enable): Bật hoặc tắt SPI
   + SPE = 0: SPI tắt.
   + SPE = 1: SPI bật. */
#define SPI_PERIPHERAL_ENABLE(__INSTANCE__)  ((__INSTANCE__)->CR1.BITS.SPE = SET)
#define SPI_PERIPHERAL_DISABLE(__INSTANCE__) ((__INSTANCE__)->CR1.BITS.SPE = RESET)

/* CRCEN (Hardware CRC Enable): Bật chế độ kiểm tra CRC dùng để kiểm tra lỗi trong giao tiếp SPI.
   + CRCEN = 0: Không dùng CRC.
   + CRCEN = 1: Bật kiểm tra CRC. */
#define SPI_CRC_ENABLE(__INSTANCE__)         ((__INSTANCE__)->CR1.BITS.CRCEN = SET)
#define SPI_CRC_DISABLE(__INSTANCE__)        ((__INSTANCE__)->CR1.BITS.CRCEN = RESET)

/* MSTR (Master Selection): Chọn chế độ Master hoặc Slave
+ MSTR = 0: Chế độ Slave.
+ MSTR = 1: Chế độ Master.*/
#define SPI_SLAVE_SELECTION(__INSTANCE__)    ((__INSTANCE__)->CR1.BITS.MSTR = RESET)
#define SPI_MASTER_SELECTION(__INSTANCE__)         \
do {                                       \
   ((__INSTANCE__)->CR1.BITS.MSTR = SET);  \
   ((__INSTANCE__)->CR1.BITS.SSI = SET);   \
} while(0)                                 \

#define SPI_SOFTWARE_NSS(__INSTANCE__)       ((__INSTANCE__)->CR1.BITS.SSM = SET)
#define SPI_HARDWARE_NSS(__INSTANCE__)       ((__INSTANCE__)->CR1.BITS.SSM = RESET)
#define SPI_SELECT_SLAVE(__INSTANCE__)       ((__INSTANCE__)->CR1.BITS.SSI = RESET)
#define SPI_DESELECT_SLAVE(__INSTANCE__)     ((__INSTANCE__)->CR1.BITS.SSI = SET)
#define SPI_ACTIVE(__INSTANCE__)             ((__INSTANCE__)->I2SCFGR.BITS.I2SMOD = RESET)

/* Chọn tốc độ baud rate cho SPI
   SPI Clock = APB Clock / (2^(BR + 1)) */
static inline void SPI_SetupBaudrate(SPI_Typedef* xSpi, SPI_Baudrate Baudrate)
{
   xSpi->CR1.BITS.BR = (uint8_t) Baudrate;
}

static inline void SPI_CRC_Polynomial(SPI_Typedef* xSpi, uint16_t val)
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
static inline void SPI_SetupNSS(SPI_Typedef* xSpi, SPI_NSS Config)
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
static inline void SPI_SetupClockMode(SPI_Typedef* xSpi, SPI_ClockMode Mode)
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
static inline void SPI_SetupFrameFormat(SPI_Typedef* xSpi, SPI_FrameFormat Format)
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
static inline void SPI_SetDirection(SPI_Typedef* xSpi, SPI_Direction dir)
{
    return;
}

static inline void SPI_SendByte(SPI_Typedef* xSpi, uint8_t data)
{
   xSpi->DR.BITS.DR = data;
}

static inline uint8_t SPI_ReadByte(SPI_Typedef* xSpi)
{
   /* Read the SPI_DR register to clear the RXNE bit */
   return xSpi->DR.BITS.DR;
}

#endif /* __HAL_SPI__ */
