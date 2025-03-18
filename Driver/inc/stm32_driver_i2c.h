#ifndef __DRIVER_I2C__
#define __DRIVER_I2C__

#include "stm32f103.h"
#include "stm32_hal_i2c.h"
#include "stm32_driver_gpio.h"

#define I2C_TIMEOUT_BUSY_FLAG       25

typedef enum {
    I2C_MODE_NONE = 0x00U,
    I2C_MODE_MASTER = 0x10U,
    I2C_MODE_SLAVE = 0x20U,
    I2C_MODE_MEM = 0x40U
} I2C_ModeTypeDef;

typedef struct {
    I2C_Typedef*    Instance;

    uint8_t*        RxBuffer;
    uint16_t        RxSize;
    uint16_t        RxCount;

    uint8_t*        TxBuffer;
    uint16_t        TxSize;
    uint16_t        TxCount;

    uint8_t         Command;
    uint8_t         Command1;
    
    FunctionalState NoStretch;
    FunctionalState GeneralCall;
    FunctionalState DualAddress;
    I2C_ModeTypeDef ModeType;

    enum {
        ADDR_7_BIT = 0,
        ADDR_10_BIT = !ADDR_7_BIT
    } SlaveMode;

    uint8_t         EventCount;
    uint16_t        DeviceAddr;
    uint16_t        OwnAddress1;
    uint8_t         OwnAddress2;
} I2C_InitHandler;

void I2C_ER_IRQHandler(I2C_InitHandler* hi2c);
void I2C_EV_IRQHandler(I2C_InitHandler* hi2c);
RETURN_STATUS I2C_Master_Transmitter(I2C_Typedef* I2Cx, uint8_t DeviceAddress, uint8_t *TxBuffer, uint16_t TxSize, uint32_t Timeout);
RETURN_STATUS I2C_Master_Transmitter_Command(I2C_Typedef* I2Cx, uint8_t DeviceAddress, uint8_t Command, uint8_t *TxBuffer, uint16_t TxSize, uint32_t Timeout);
RETURN_STATUS I2C_Master_Receiver(I2C_Typedef* I2Cx, uint8_t DeviceAddress, uint8_t Command, uint8_t* RxBuffer, uint16_t RxSize, uint32_t Timeout);
RETURN_STATUS I2C_Master_Transmitter_IT(I2C_InitHandler* hi2c, uint8_t DeviceAddress, uint8_t *TxBuffer, uint16_t TxSize);
RETURN_STATUS I2C_Master_Receiver_IT(I2C_InitHandler* hi2c, uint8_t DeviceAddress, uint8_t *RxBuffer, uint16_t RxSize);
RETURN_STATUS I2C_Slave_Transmitter_IT(I2C_InitHandler* hi2c, uint8_t* TxBuffer, uint16_t TxSize);
RETURN_STATUS I2C_Slave_Receiver_IT(I2C_InitHandler* hi2c, uint8_t* TxBuffer, uint16_t TxSize);

#endif /* __DRIVER_I2C__ */
