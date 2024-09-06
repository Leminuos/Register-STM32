#ifndef __DRIVER_I2C__
#define __DRIVER_I2C__

#include "stm32f103.h"
#include "stm32_hal_i2c.h"

RETURN_STATUS I2C_Master_Transmitter(I2C_Typedef* I2Cx, uint32_t DeviceAddress, uint8_t *TxBuffer, uint16_t TxSize, uint32_t Timeout);
RETURN_STATUS I2C_Master_Receivei(I2C_Typedef* I2Cx, uint32_t DeviceAddress, uint8_t Command, uint8_t* RxBuffer, uint16_t RxSize, uint32_t Timeout);

#endif /* __DRIVER_I2C__ */
