#ifndef __DRIVER_SPI__
#define __DRIVER_SPI__

#include "stm32_hal_spi.h"

void SPI_Init(SPI_Typedef* xSpi);
uint8_t SPI_ReceiveByte(SPI_Typedef* xSpi);
void SPI_WriteByte(SPI_Typedef* xSpi, uint8_t val);
uint8_t SPI_Transfer(SPI_Typedef* xSpi, uint8_t val);
void SPI_Transmit(SPI_Typedef* xSpi, const uint8_t *TxBuffer, uint16_t TxCount);
void SPI_TransmitReceive(SPI_Typedef* xSpi, const uint8_t *TxBuffer, uint8_t* RxBuffer, uint16_t Size);

#endif /* __DRIVER_SPI__ */
