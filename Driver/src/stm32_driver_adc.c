#include "stm32_driver_adc.h"

void ADC_Prescaler(uint8_t pres)
{
    RCC->CFGR.BITS.ADCPRE = pres;
}

void ADC_Init()
{
    // Bật clock cho GPIOB và ADC1
    RCC->APB2ENR.BITS.IOPBEN = 1;
    RCC->APB2ENR.BITS.ADC1EN = 1;

    // Cấu hình PB1 thành chế độ analog (MODER = 11)
    GPIOB->CRL.BITS.MODE1 = 0x00;
    GPIOB->CRL.BITS.CNF1 = 0x00;

    // Cấu hình ADC1
    ADC1->ADC_CR1.WORD = 0x00;
    ADC1->ADC_SMPR2.BITS.SMP1 = 0x03;
    ADC1->ADC_SQR1.BITS.L = 0x00;   // 1 kênh chuyển đổi
    ADC1->ADC_SQR3.BITS.SQ1 = 0x09; // Kênh 9
    ADC1->ADC_CR2.BITS.ADON = 0x01;
}

uint16_t ADC_ReadData(void)
{
    ADC1->ADC_CR2.BITS.ADON = 0x01;
    while (!(ADC1->ADC_SR.WORD & (1 << 1)));
    return ADC1->ADC_DR.BITS.REGULARDATA;
}

