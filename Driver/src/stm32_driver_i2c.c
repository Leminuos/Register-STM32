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

extern void delay(uint32_t);

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
    I2Cx->CR1.BITS.SWRST = SET;         // Reset bit BUSY flag => master mode entry
    I2Cx->CR1.BITS.SWRST = RESET;
    
    I2C_ConfigSpeed(I2Cx, 36000000, I2C_MAX_SPEED_STANDARD, 0);
    
    I2Cx->OAR1.REG = 1U << 14;
    
    I2C_ENABLE_PERIPHERAL(I2Cx);
    if (I2Cx->SR2.BITS.BUSY == SET) return;
    
    I2C_ENABLE_START(I2Cx);
    while (!I2Cx->SR1.BITS.SB)
    {
        if (I2Cx->SR1.BITS.AF == SET)
        {
            I2C_ENABLE_STOP(I2Cx);
            return;
        }
    }
    
    I2C_SEND_DATA(I2Cx, 0x68 << 1);
    while (!I2Cx->SR1.BITS.ADDR)    // Thêm volatile vào biến để có thể detect được biến này
                                    // Vì sự thay đổi của hardware là quá nhanh
    {
        if (I2Cx->SR1.BITS.AF == SET)
        {
            I2C_ENABLE_STOP(I2Cx);
            return;
        }
    }
    
    /* Clear the I2C ADDR pending flag */
    I2C_CLEAR_ADDRFLAG(I2Cx);
    
    while (!I2Cx->SR1.BITS.TxE)
    {
        if (I2Cx->SR1.BITS.AF == SET)
        {
            I2C_ENABLE_STOP(I2Cx);
            return;
        }
    }
    I2C_SEND_DATA(I2Cx, 0x01);
    
    while (!I2Cx->SR1.BITS.TxE)
    {
        if (I2Cx->SR1.BITS.AF == SET)
        {
            I2C_ENABLE_STOP(I2Cx);
            return;
        }
    }
    I2C_SEND_DATA(I2Cx, 0x48);
    
    while (!I2Cx->SR1.BITS.BTF)
    {
        if (I2Cx->SR1.BITS.AF == SET)
        {
            I2C_ENABLE_STOP(I2Cx);
            return;
        }
    }
    
    I2C_ENABLE_STOP(I2Cx);
}

RERURN_STATUS I2C_Master_Transmitter(I2C_Typedef* I2Cx)
{
    return STATUS_SUCCES;
}
