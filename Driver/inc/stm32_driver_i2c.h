#ifndef __DRIVER_I2C__
#define __DRIVER_I2C__

#include "stm32f103.h"
#include "stm32_hal_i2c.h"

RERURN_STATUS I2C_Master_Transmitter(I2C_Typedef* I2Cx);
RERURN_STATUS I2C_Master_Receivei(I2C_Typedef* I2Cx);

#endif /* __DRIVER_I2C__ */
