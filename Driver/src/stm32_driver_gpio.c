#include "stm32_driver_gpio.h"

void GPIO_Init(GPIO_Typedef* Port, GPIO_InitTypeDef* GPIO_InitStruct)
{
    uint8_t     idxpin = 0;
    uint8_t     tempbit = 0;
    uint32_t    tempreg = 0;
    uint32_t    pinmask = 0;
    uint32_t    currentmode = 0;

    if (Port == GPIOA)      RCC->APB2ENR.BITS.IOPAEN = (uint32_t) SET;
    else if (Port == GPIOB) RCC->APB2ENR.BITS.IOPBEN = (uint32_t) SET;
    else if (Port == GPIOC) RCC->APB2ENR.BITS.IOPCEN = (uint32_t) SET;
    else if (Port == GPIOD) RCC->APB2ENR.BITS.IOPDEN = (uint32_t) SET;
    else if (Port == GPIOE) RCC->APB2ENR.BITS.IOPEEN = (uint32_t) SET;
    else if (Port == GPIOF) RCC->APB2ENR.BITS.IOPFEN = (uint32_t) SET;
    else if (Port == GPIOG) RCC->APB2ENR.BITS.IOPGEN = (uint32_t) SET;
    
    currentmode = ((uint32_t)(GPIO_InitStruct->GPIO_Mode) & (uint32_t) 0x0F);

    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
    { 
        currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
    }

    if ((GPIO_InitStruct->GPIO_Pin & 0x00FF) != 0)      // IRL
    {
        tempreg = Port->CRL.REG;

        for (idxpin = 0; idxpin < 8; ++idxpin)
        {
            tempbit = (((uint32_t)0x01U) << idxpin);

            if ((GPIO_InitStruct->GPIO_Pin & tempbit) == tempbit)
            {
                tempbit = idxpin << 2;
                pinmask = ((uint32_t)0x0F) << tempbit;
                tempreg &= ~pinmask;
                tempreg = tempreg | (currentmode << tempbit);
            }
        }

        Port->CRL.REG = tempreg;
    }
    
    if ((GPIO_InitStruct->GPIO_Pin & 0xFF00) != 0)      // IRH
    {
        tempreg = Port->CRH.REG;

        for (idxpin = 0; idxpin < 8; ++idxpin)
        {
            tempbit = (((uint32_t)0x01U) << (idxpin + 0x08));

            if ((GPIO_InitStruct->GPIO_Pin & tempbit) == tempbit)
            {
                tempbit = idxpin << 2;
                pinmask = ((uint32_t)0x0F) << tempbit;
                tempreg &= ~pinmask;
                tempreg = tempreg | (currentmode << tempbit);
            }
        }
        
        Port->CRH.REG = tempreg;
    }

    tempreg = Port->ODR.REG;

    if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
    {
        CLEAR_BIT(tempreg, (uint32_t) GPIO_InitStruct->GPIO_Pin);
    }
    else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
    {
        SET_BIT(tempreg, (uint32_t) GPIO_InitStruct->GPIO_Pin);
    }
    else
    {
        // Do something
    }

    Port->ODR.REG = tempreg;
}

void GPIO_SetBit(GPIO_Typedef* Port, uint16_t Pin)
{
    SET_BIT(Port->BSRR.REG, Pin);
}

void GPIO_ResetBit(GPIO_Typedef* Port, uint16_t Pin)
{
    CLEAR_BIT(Port->BRR.REG, Pin);
}

uint8_t GPIO_ReadInputDataBit(GPIO_Typedef* Port, uint16_t Pin)
{
    return ((uint8_t) READ_BIT(Port->IDR.REG, Pin));
}
