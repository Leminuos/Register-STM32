#include "stm32_driver_i2c.h"

#define GPIO_INPUT_ANALOG               0x00
#define GPIO_INPUT_FLOATING             0x01
#define GPIO_INPUT_PP_PU                0x03
#define GPIO_INPUT_PP_PD                0x03

#define GPIO_PURPOSE_OUTPUT_PP_10MHZ    0x00
#define GPIO_PURPOSE_OUTPUT_OD_10MHZ    0x01
#define GPIO_AF_OUTPUT_PP_10MHZ         0x02
#define GPIO_AF_OUTPUT_OD_10MHZ         0x03

#define GPIO_PURPOSE_OUTPUT_PP_2MHZ     0x00
#define GPIO_PURPOSE_OUTPUT_OD_2MHZ     0x01
#define GPIO_AF_OUTPUT_PP_2MHZ          0x02
#define GPIO_AF_OUTPUT_OD_2MHZ          0x03

#define GPIO_PURPOSE_OUTPUT_PP_50MHZ    0x00
#define GPIO_PURPOSE_OUTPUT_OD_50MHZ    0x01
#define GPIO_AF_OUTPUT_PP_50MHZ         0x02
#define GPIO_AF_OUTPUT_OD_50MHZ         0x03

void I2C_Init(I2C_InitHandler* pI2C)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if (pI2C->Instance == I2C1)
    {
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_Init(GPIOB, &GPIO_InitStruct);

        RCC->APB1ENR.BITS.I2C1EN = SET;

        NVIC_SetPriority(I2C1_EV_IRQn, 0x00);
        NVIC_EnableIRQ(I2C1_EV_IRQn);

        NVIC_SetPriority(I2C1_ER_IRQn, 0x00);
        NVIC_EnableIRQ(I2C1_ER_IRQn);
    }
    else if (pI2C->Instance == I2C2)
    {
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_Init(GPIOB, &GPIO_InitStruct);
        
        RCC->APB1ENR.BITS.I2C2EN = SET;

        NVIC_SetPriority(I2C2_EV_IRQn, 0x00);
        NVIC_EnableIRQ(I2C2_EV_IRQn);

        NVIC_SetPriority(I2C2_ER_IRQn, 0x00);
        NVIC_EnableIRQ(I2C2_ER_IRQn);
    }
    
    I2C_DISABLE_PERIPHERAL(pI2C->Instance);

    // Reset bit BUSY flag => master mode entry
    I2C_SOFTWARE_RESET(pI2C->Instance);

    I2C_ConfigSpeed(pI2C->Instance, 36000000, I2C_MAX_SPEED_STANDARD, 0);

    I2C_NO_STRETCH(pI2C->Instance, pI2C->NoStretch);

    I2C_GENERAL_CALL(pI2C->Instance, pI2C->GeneralCall);

    I2C_ADDR_MODE(pI2C->Instance, pI2C->SlaveMode);

    I2C_OWN_ADDR_1(pI2C->Instance, pI2C->OwnAddress1);

    I2C_OWN_ADDR_2(pI2C->Instance, pI2C->OwnAddress2);

    I2C_ENABLE_PERIPHERAL(pI2C->Instance);
}
 
RETURN_STATUS I2C_Master_Transmitter(I2C_Typedef* I2Cx, uint8_t DeviceAddress, uint8_t *TxBuffer, uint16_t TxSize, uint32_t Timeout)
{
    uint32_t TickStart  = GetCounterTick();
    uint8_t Address     = DeviceAddress << 1;
    uint8_t u8Data      = 0;

    if (I2C_CHECK_PERIPHERAL(I2Cx) == RESET)
    {
        I2C_ENABLE_PERIPHERAL(I2Cx);
    }
    
    if (HAL_STATUS_ERROR == I2C_WaitBusyUntilTimeout(I2Cx, Timeout, TickStart))
    {
        return HAL_STATUS_BUSY;
    }
    
    I2C_ENABLE_START(I2Cx);
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_SB))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_START;
    }
    
     I2C_SEND_DATA(I2Cx, Address);
    
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_ADDR))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_ADDRESS;
    }
    
    /* Clear the I2C ADDR pending flag */
    I2C_CLEAR_ADDRFLAG(I2Cx);

    while (TxSize > 0U)
    {
        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_TXE))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_TXE;
        }
        
        u8Data = *TxBuffer;

        /* Increment Buffer pointer */
        --TxSize;

        /* Update counter */
        ++TxBuffer;

        /* Write data to DR */
        I2C_SEND_DATA(I2Cx, u8Data);

        if (I2C_CHK_FLAG(I2Cx, I2C_FLAG_BTF) && TxSize > 0U)
        {
            u8Data = *TxBuffer;

            /* Increment Buffer pointer */
            ++TxBuffer;

            /* Update counter */
            --TxSize;

            /* Write data to DR */
            I2C_SEND_DATA(I2Cx, u8Data);
        }
    }

    /* Wait until BTF flag is set */
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_BTF))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_BTF;
    }

    I2C_ENABLE_STOP(I2Cx);
    
    return HAL_STATUS_SUCCESS;
}

RETURN_STATUS I2C_Master_Transmitter_Command(I2C_Typedef* I2Cx, uint8_t DeviceAddress, uint8_t Command, uint8_t *TxBuffer, uint16_t TxSize, uint32_t Timeout)
{
    uint32_t TickStart  = GetCounterTick();
    uint8_t Address     = DeviceAddress << 1;
    uint8_t u8Data      = 0;

    if (I2C_CHECK_PERIPHERAL(I2Cx) == RESET)
    {
        I2C_ENABLE_PERIPHERAL(I2Cx);
    }
    
    if (HAL_STATUS_ERROR == I2C_WaitBusyUntilTimeout(I2Cx, Timeout, TickStart))
    {
        return HAL_STATUS_BUSY;
    }
    
    I2C_ENABLE_START(I2Cx);
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_SB))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_START;
    }
    
     I2C_SEND_DATA(I2Cx, Address);
    
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_ADDR))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_ADDRESS;
    }
    
    /* Clear the I2C ADDR pending flag */
    I2C_CLEAR_ADDRFLAG(I2Cx);

    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_TXE))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_TXE;
    }

    /* Write data to DR */
    I2C_SEND_DATA(I2Cx, Command);

    while (TxSize > 0U)
    {
        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_TXE))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_TXE;
        }
        
        u8Data = *TxBuffer;

        /* Increment Buffer pointer */
        --TxSize;

        /* Update counter */
        ++TxBuffer;

        /* Write data to DR */
        I2C_SEND_DATA(I2Cx, u8Data);

        if (I2C_CHK_FLAG(I2Cx, I2C_FLAG_BTF) && TxSize > 0U)
        {
            u8Data = *TxBuffer;

            /* Increment Buffer pointer */
            ++TxBuffer;

            /* Update counter */
            --TxSize;

            /* Write data to DR */
            I2C_SEND_DATA(I2Cx, u8Data);
        }
    }

    /* Wait until BTF flag is set */
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_BTF))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_BTF;
    }

    I2C_ENABLE_STOP(I2Cx);
    
    return HAL_STATUS_SUCCESS;
}

RETURN_STATUS I2C_Master_Receiver(I2C_Typedef* I2Cx, uint8_t DeviceAddress, uint8_t Command, uint8_t* RxBuffer, uint16_t RxSize, uint32_t Timeout)
{
    uint32_t TickStart  = GetCounterTick();
    uint8_t Address     = DeviceAddress << 1;

    if (I2C_CHECK_PERIPHERAL(I2Cx) == RESET)
    {
        I2C_ENABLE_PERIPHERAL(I2Cx);
    }

    if (HAL_STATUS_ERROR == I2C_WaitBusyUntilTimeout(I2Cx, Timeout, TickStart))
    {
        return HAL_STATUS_BUSY;
    }

    I2C_ENABLE_START(I2Cx);
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_SB))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_START;
    }
    
    I2C_SEND_DATA(I2Cx, Address); 
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_ADDR))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_ADDRESS;
    }
    
    /* Clear the I2C ADDR pending flag */
    I2C_CLEAR_ADDRFLAG(I2Cx);

    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_TXE))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_TXE;
    }

    I2C_SEND_DATA(I2Cx, Command);
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_TXE))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_TXE;
    }

    /* Repeat Start */
    I2C_ENABLE_START(I2Cx);
    if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_SB))
    {
        I2C_ENABLE_STOP(I2Cx);
        return I2C_STATUS_ERR_START;
    }

    Address = Address | 1;

    if (1U == RxSize)
    {
        I2C_SEND_DATA(I2Cx, Address); 

        /* Program ACK = 0 */
        I2C_GENERATE_NACK(I2Cx);

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_ADDR))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_ADDRESS;
        }

        /* Clear the I2C ADDR pending flag */
        I2C_CLEAR_ADDRFLAG(I2Cx);

        I2C_ENABLE_STOP(I2Cx);

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_RXNE))
            return I2C_STATUS_ERR_RXNE;

        /* Read data from DR */
        (*RxBuffer) = I2C_READ_DATA(I2Cx);

        /* Update counter */
        --RxSize;
    }
    else if (2U == RxSize)
    {
        I2C_ENABLE_POS(I2Cx);
        I2C_GENERATE_ACK(I2Cx);

        I2C_SEND_DATA(I2Cx, Address);

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_ADDR))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_ADDRESS;
        }

        /* Clear the I2C ADDR pending flag */
        I2C_CLEAR_ADDRFLAG(I2Cx);

        /* Clear the I2C ACK pending flag */
        I2C_GENERATE_NACK(I2Cx);

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_BTF))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_BTF;
        }

        I2C_ENABLE_STOP(I2Cx);

        while (RxSize > 0U)
        {
            /* Read data from DR */
            (*RxBuffer) = I2C_READ_DATA(I2Cx);

            /* Increment Buffer pointer */
            ++RxBuffer;

            /* Update counter */
            --RxSize;
        }

        I2C_DISABLE_POS(I2Cx);
    }
    else
    {
        I2C_GENERATE_ACK(I2Cx);

        I2C_SEND_DATA(I2Cx, Address);

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_ADDR))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_ADDRESS;
        }

        /* Clear the I2C ADDR pending flag */
        I2C_CLEAR_ADDRFLAG(I2Cx);

        while (RxSize > 3U)
        {
            if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_RXNE))
            {
                I2C_ENABLE_STOP(I2Cx);
                return I2C_STATUS_ERR_RXNE;
            }

            /* Read data from DR */
            (*RxBuffer) = I2C_READ_DATA(I2Cx);
            
            /* Increment Buffer pointer */
            ++RxBuffer;

            /* Update counter */
            --RxSize;
        }

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_BTF))
        {
            I2C_ENABLE_STOP(I2Cx);
            return I2C_STATUS_ERR_BTF;
        }

        /* Clear the I2C ACK pending flag */
        I2C_GENERATE_NACK(I2Cx);

        /* Read data N-2 from DR */
        (*RxBuffer) = I2C_READ_DATA(I2Cx);

        /* Increment Buffer pointer */
        ++RxBuffer;

        /* Update counter */
        --RxSize;

        I2C_ENABLE_STOP(I2Cx);

        /* Read data N-1 from DR */
        (*RxBuffer) = I2C_READ_DATA(I2Cx);

        /* Increment Buffer pointer */
        ++RxBuffer;

        /* Update counter */
        --RxSize;

        if (HAL_STATUS_ERROR == I2C_WaitOnFlagUntilTimeout(I2Cx, Timeout, TickStart, I2C_FLAG_RXNE))
            return I2C_STATUS_ERR_RXNE;

        /* Read data N from DR */
        (*RxBuffer) = I2C_READ_DATA(I2Cx);

        /* Increment Buffer pointer */
        ++RxBuffer;

        /* Update counter */
        --RxSize;
    }

    return HAL_STATUS_SUCCESS;
}

RETURN_STATUS I2C_Master_Transmitter_IT(I2C_InitHandler* hi2c, uint8_t DeviceAddress, uint8_t *TxBuffer, uint16_t TxSize)
{
    uint32_t count = 0U;

    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (36000000U / 25U / 1000U);

    do
    {
        --count;

        if (count == 0)
        {
            return HAL_STATUS_BUSY;
        }
    } while (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BUSY) == SET);

    hi2c->ModeType      = I2C_MODE_MASTER;
    hi2c->EventCount    = 0U;
    hi2c->TxBuffer      = TxBuffer;
    hi2c->TxSize        = TxSize;
    hi2c->TxCount       = hi2c->TxSize;
    hi2c->DeviceAddr    = DeviceAddress;

    I2C_DISABLE_POS(hi2c->Instance);

    I2C_ENABLE_IT(hi2c->Instance, (I2C_IT_EVENT | I2C_IT_BUFFER | I2C_IT_ERROR));

    I2C_ENABLE_START(hi2c->Instance);

    return HAL_STATUS_SUCCESS;
}

RETURN_STATUS I2C_Master_Receiver_IT(I2C_InitHandler* hi2c, uint8_t DeviceAddress, uint8_t *RxBuffer, uint16_t RxSize)
{
    uint32_t count = 0U;
    
    /* Wait until BUSY flag is reset */
    count = I2C_TIMEOUT_BUSY_FLAG * (36000000U / 25U / 1000U);

    do
    {
        --count;

        if (count == 0)
        {
            return HAL_STATUS_BUSY;
        }
    } while (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BUSY) == SET);

    hi2c->ModeType      = I2C_MODE_MASTER;
    hi2c->EventCount    = 1U;
    hi2c->RxBuffer      = RxBuffer;
    hi2c->RxSize        = RxSize;
    hi2c->RxCount       = hi2c->RxSize;
    hi2c->DeviceAddr    = DeviceAddress;

    I2C_DISABLE_POS(hi2c->Instance);


    I2C_ENABLE_IT(hi2c->Instance, (I2C_IT_EVENT | I2C_IT_BUFFER | I2C_IT_ERROR));

    I2C_ENABLE_START(hi2c->Instance);

    I2C_GENERATE_ACK(hi2c->Instance);
    
    return HAL_STATUS_SUCCESS;
}

RETURN_STATUS I2C_Slave_Transmitter_IT(I2C_InitHandler* hi2c, uint8_t* TxBuffer, uint16_t TxSize)
{
    hi2c->ModeType = I2C_MODE_SLAVE;
    hi2c->TxBuffer = TxBuffer;
    hi2c->TxSize = TxSize;
    hi2c->TxCount = hi2c->TxSize;

    I2C_DISABLE_POS(hi2c->Instance);

    I2C_GENERATE_ACK(hi2c->Instance);

    I2C_ENABLE_IT(hi2c->Instance, (I2C_IT_EVENT | I2C_IT_BUFFER | I2C_IT_ERROR));

    return HAL_STATUS_SUCCESS;
}

RETURN_STATUS I2C_Slave_Receiver_IT(I2C_InitHandler* hi2c, uint8_t* RxBuffer, uint16_t RxSize)
{
    hi2c->ModeType = I2C_MODE_SLAVE;
    hi2c->RxBuffer = RxBuffer;
    hi2c->RxSize = RxSize;
    hi2c->RxCount = hi2c->RxSize;

    I2C_DISABLE_POS(hi2c->Instance);

    I2C_GENERATE_ACK(hi2c->Instance);

    I2C_ENABLE_IT(hi2c->Instance, (I2C_IT_EVENT | I2C_IT_BUFFER | I2C_IT_ERROR));

    return HAL_STATUS_SUCCESS;
}

static void I2C_Master_SB(I2C_InitHandler* hi2c)
{
    uint8_t DeviceAddr = hi2c->DeviceAddr << 1;

    if (0U == hi2c->EventCount)
    {
        I2C_SEND_DATA(hi2c->Instance, DeviceAddr);
    }
    else if ((1U == hi2c->EventCount))
    {
        DeviceAddr = DeviceAddr | 1;
        I2C_SEND_DATA(hi2c->Instance, DeviceAddr);
    }
    else
    {
        // Do something
    }
}

static void I2C_Master_ADDR(I2C_InitHandler* hi2c)
{
    if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TRA) == RESET)
    {
        if (1U == hi2c->RxSize)
        {
            I2C_GENERATE_NACK(hi2c->Instance);
            
            I2C_CLEAR_ADDRFLAG(hi2c->Instance);

            I2C_ENABLE_STOP(hi2c->Instance);
        }
        else if (2U == hi2c->RxSize)
        {
            I2C_ENABLE_POS(hi2c->Instance);

            I2C_CLEAR_ADDRFLAG(hi2c->Instance);

            I2C_GENERATE_NACK(hi2c->Instance);
        }
        else I2C_CLEAR_ADDRFLAG(hi2c->Instance);
    }
    else I2C_CLEAR_ADDRFLAG(hi2c->Instance);
}

static void I2C_Master_TXE(I2C_InitHandler* hi2c)
{
    uint8_t u8Data = 0;

    if (hi2c->TxCount > 0U)
    {
        u8Data = *(hi2c->TxBuffer);
        I2C_SEND_DATA(hi2c->Instance, u8Data);
        ++hi2c->TxBuffer;
        --hi2c->TxCount;

        if (0U == hi2c->TxCount) I2C_CLEAR_IT(hi2c->Instance, I2C_IT_BUFFER);

        return;
    }

    I2C_ENABLE_STOP(hi2c->Instance);
}

static void I2C_Master_RXNE(I2C_InitHandler* hi2c)
{
    if (2U == hi2c->RxSize)
    {
        I2C_CLEAR_IT(hi2c->Instance, I2C_IT_BUFFER);
        return;
    }

    if (hi2c->RxCount > 0U)
    {
        *(hi2c->RxBuffer) = I2C_READ_DATA(hi2c->Instance);
        ++hi2c->RxBuffer;
        --hi2c->RxCount;

        if (3U == hi2c->RxCount) I2C_CLEAR_IT(hi2c->Instance, I2C_IT_BUFFER);

        return;
    }

    I2C_ENABLE_STOP(hi2c->Instance);
}

static void I2C_Master_Transmitter_BTF(I2C_InitHandler* hi2c)
{
    uint8_t u8Data = 0;

    if (hi2c->TxCount > 0U)
    {
        u8Data = *(hi2c->TxBuffer);
        I2C_SEND_DATA(hi2c->Instance, u8Data);
        ++hi2c->TxBuffer;
        --hi2c->TxCount;
        return;
    }
    
    I2C_CLEAR_IT(hi2c->Instance, (I2C_IT_ERROR | I2C_IT_EVENT));
    I2C_ENABLE_STOP(hi2c->Instance);
}

static void I2C_Master_Receiver_BTF(I2C_InitHandler* hi2c)
{
    if (3U == hi2c->RxCount)
    {        
        I2C_GENERATE_NACK(hi2c->Instance);

        *(hi2c->RxBuffer) = I2C_READ_DATA(hi2c->Instance);
        ++hi2c->RxBuffer;
        --hi2c->RxCount;

        I2C_ENABLE_STOP(hi2c->Instance);

        *(hi2c->RxBuffer) = I2C_READ_DATA(hi2c->Instance);
        ++hi2c->RxBuffer;
        --hi2c->RxCount;

        I2C_CLEAR_IT(hi2c->Instance, (I2C_IT_EVENT | I2C_IT_ERROR));

        return;
    }

    if (2U == hi2c->RxSize)
    {
        I2C_ENABLE_STOP(hi2c->Instance);

        while (hi2c->RxCount > 0U)
        {
            *(hi2c->RxBuffer) = I2C_READ_DATA(hi2c->Instance);
            ++hi2c->RxBuffer;
            --hi2c->RxCount;
        }

        I2C_DISABLE_POS(hi2c->Instance);
        I2C_CLEAR_IT(hi2c->Instance, (I2C_IT_EVENT | I2C_IT_ERROR));

        return;
    }

    I2C_ENABLE_STOP(hi2c->Instance);
}

static void I2C_Slave_ADDR(I2C_InitHandler* hi2c)
{
    I2C_CLEAR_ADDRFLAG(hi2c->Instance);
}

static void I2C_Slave_STOPF(I2C_InitHandler* hi2c)
{
    /* Clear STOPF flag */
    I2C_CLEAR_STOPF(hi2c->Instance);

    /* Disable Acknowledge */
    I2C_GENERATE_NACK(hi2c->Instance);

    /* Disable EVT, BUF and ERR interrupt */
    I2C_CLEAR_IT(hi2c->Instance, (I2C_IT_ERROR | I2C_IT_EVENT | I2C_IT_BUFFER));
}

static void I2C_Slave_TXE(I2C_InitHandler* hi2c)
{
    uint8_t u8Data = 0;

    if (hi2c->TxCount > 0U)
    {
        u8Data = *(hi2c->TxBuffer);
        I2C_SEND_DATA(hi2c->Instance, u8Data);
        ++hi2c->TxBuffer;
        --hi2c->TxCount;

        if (0U == hi2c->TxCount) I2C_CLEAR_IT(hi2c->Instance, I2C_IT_BUFFER);
    }
}

static void I2C_Slave_Transmitter_BTF(I2C_InitHandler* hi2c)
{
    uint8_t u8Data = 0;

    if (hi2c->TxCount > 0U)
    {
        u8Data = *(hi2c->TxBuffer);
        I2C_SEND_DATA(hi2c->Instance, u8Data);
        ++hi2c->TxBuffer;
        --hi2c->TxCount;
    }
}

static void I2C_Slave_RXNE(I2C_InitHandler* hi2c)
{
    if (hi2c->RxCount > 0U)
    {
        *(hi2c->RxBuffer) = I2C_READ_DATA(hi2c->Instance) + 1U;
        ++hi2c->RxBuffer;
        --hi2c->RxCount;

        if (0U == hi2c->RxCount) I2C_CLEAR_IT(hi2c->Instance, I2C_IT_BUFFER);
    }
}

static void I2C_Slave_Receiver_BTF(I2C_InitHandler* hi2c)
{
    if (hi2c->RxCount > 0U)
    {
        *(hi2c->RxBuffer) = I2C_READ_DATA(hi2c->Instance) + 1U;
        ++hi2c->RxBuffer;
        --hi2c->RxCount;
    }
}

void I2C_EV_IRQHandler(I2C_InitHandler* hi2c)
{
    if (I2C_CHK_IT(hi2c->Instance, I2C_IT_EVENT) != RESET)
    {
        if (hi2c->ModeType == I2C_MODE_MASTER)
        {
            if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_SB) != RESET)
            {
                I2C_Master_SB(hi2c);
            }
            else if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_ADDR) != RESET)
            {
                I2C_Master_ADDR(hi2c);
            }
            /* I2C in mode Transmitter -----------------------------------------------*/
            else if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TRA) != RESET)
            {
                if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TXE) != RESET) && (I2C_CHK_IT(hi2c->Instance, I2C_IT_BUFFER) != RESET))
                {
                    I2C_Master_TXE(hi2c);
                }
                else if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF)) != RESET)
                {
                    I2C_Master_Transmitter_BTF(hi2c);
                }
                else
                {
                    // Do something
                }
            }
            /* I2C in mode Receiver -----------------------------------------------*/
            else if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TRA) == RESET)
            {
                if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_RXNE) != RESET) &&   \
                    (I2C_CHK_IT(hi2c->Instance, I2C_IT_BUFFER) != RESET) &&     \
                    (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF)) == RESET)
                {
                    I2C_Master_RXNE(hi2c);
                }
                else if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF)) != RESET)
                {
                    I2C_Master_Receiver_BTF(hi2c);
                }
                else
                {
                    // Do something
                }
            }
            else
            {
                // Do something
            }
        }
        else if (hi2c->ModeType == I2C_MODE_SLAVE)
        {
            if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_ADDR) != RESET)
            {
                I2C_Slave_ADDR(hi2c);
            }
            else if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_STOPF) != RESET)
            {
                I2C_Slave_STOPF(hi2c);
            }
            /* I2C in mode Transmitter -----------------------------------------------*/
            else if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TRA) != RESET)
            {
                if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TXE) != RESET) && \
                    (I2C_CHK_IT(hi2c->Instance, I2C_IT_BUFFER) != RESET) && \
                    (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF) == RESET))
                {
                    I2C_Slave_TXE(hi2c);
                }
                else if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF)) != RESET)
                {
                    I2C_Slave_Transmitter_BTF(hi2c);
                }
                else
                {
                    // Do something
                }
            }
            /* I2C in mode Receiver -----------------------------------------------*/
            else if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_TRA) == RESET)
            {
                if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_RXNE) != RESET) &&   \
                    (I2C_CHK_IT(hi2c->Instance, I2C_IT_BUFFER) != RESET) &&     \
                    (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF) == RESET))
                {
                    I2C_Slave_RXNE(hi2c);
                }
                else if ((I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_BTF)) != RESET)
                {
                    I2C_Slave_Receiver_BTF(hi2c);
                }
                else
                {
                    // Do something
                }
            }
            else
            {
                // Do something
            }
        }
        else
        {
            // Do something
        }
    }
}

void I2C_Error_AF(I2C_InitHandler* hi2c)
{
    /* Clear AF flag */
    I2C_CLEAR_AF(hi2c->Instance);

    /* Disable Acknowledge */
    I2C_GENERATE_NACK(hi2c->Instance);

    /* Disable EVT, BUF and ERR interrupt */
    I2C_CLEAR_IT(hi2c->Instance, (I2C_IT_ERROR | I2C_IT_EVENT | I2C_IT_BUFFER));

    if (hi2c->ModeType == I2C_MODE_MASTER)
    {
        I2C_ENABLE_STOP(hi2c->Instance);
    }
}

void I2C_ER_IRQHandler(I2C_InitHandler* hi2c)
{
    if (I2C_CHK_IT(hi2c->Instance, I2C_IT_ERROR) != RESET)
    {
        if (I2C_CHK_FLAG(hi2c->Instance, I2C_FLAG_AF) != RESET)
        {
            I2C_Error_AF(hi2c);
        }
    }
}
