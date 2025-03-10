#ifndef __DRIVER_SPI__
#define __DRIVER_SPI__

#include "stm32_hal_spi.h"

uint8_t SPI_Transfer(SPI_Typedef* xSpi, uint8_t val);
void SPI_Init(SPI_Typedef* xSpi);

#endif /* __DRIVER_SPI__ */
