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

void I2C_Init(I2C_Typedef* I2Cx)
{   
    if (I2Cx == I2C1)
    {
        RCC->APB2ENR.BITS.IOPBEN = SET;
        
        GPIOB->CRL.BITS.CNF7 = GPIO_AF_OUTPUT_OD_50MHZ;
        GPIOB->CRL.BITS.MODE7 = 0x03;
        
        GPIOB->CRL.BITS.CNF6 = GPIO_AF_OUTPUT_OD_50MHZ;
        GPIOB->CRL.BITS.MODE6 = 0x03;
        
        RCC->APB1ENR.BITS.I2C1EN = SET;
    }
    
    I2C_DISABLE_PERIPHERAL(I2Cx);

    // Reset bit BUSY flag => master mode entry
    I2C_SOFTWARE_RESET(I2Cx);

    I2Cx->OAR1.REG = 1U << 14;
    
    I2C_ConfigSpeed(I2Cx, 36000000, I2C_MAX_SPEED_STANDARD, 0);
    
    I2C_ENABLE_PERIPHERAL(I2Cx);
}

RETURN_STATUS I2C_Master_Transmitter(I2C_Typedef* I2Cx, uint32_t DeviceAddress, uint8_t *TxBuffer, uint16_t TxSize, uint32_t Timeout)
{
    uint32_t TickStart  = HAL_GetTick();
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

        if (CLK_FLAG_I2C(I2Cx, I2C_FLAG_BTF) && TxSize > 0U)
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

RETURN_STATUS I2C_Master_Receivei(I2C_Typedef* I2Cx, uint32_t DeviceAddress, uint8_t Command, uint8_t* RxBuffer, uint16_t RxSize, uint32_t Timeout)
{
    uint32_t TickStart  = HAL_GetTick();
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
