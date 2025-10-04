#include <stdint.h>
#include "stm32_driver_spi.h"

#define ENC_RX_START             0x0000U
#define ENC_RX_END               0x0BFFU
#define ENC_TX_START             0x0C00U
#define ENC_TX_END               0x1FFFU

extern void delay(uint16_t mDelay);

static inline void enc_cs_low(void)
{
    GPIOB->ODR.BITS.ODR12  =  0;
}

static inline void enc_cs_high(void)
{
    GPIOB->ODR.BITS.ODR12  =  1;
}

static inline void enc_delay_ms(uint32_t ms)
{
    delay(ms);
}

static inline uint8_t enc_spi_transfer(uint8_t data)
{
    return SPI_Transfer(SPI2, data);
}
