#include "stm32_hal_i2c.h"

void I2C_ConfigSpeed(I2C_Typedef* I2Cx, uint32_t PeriphClock, uint32_t ClockSpeed, uint32_t DutyCycle)
{
    register uint32_t freq = 0;
    register uint32_t clockconfig = 0;
    
    freq = I2C_PCLK_TO_FREQ(PeriphClock);
    
    I2Cx->CR2.BITS.FREQ = (uint8_t) freq;
    I2Cx->TRISE = I2C_RISE_TIME(freq, ClockSpeed);
    
    if (ClockSpeed > I2C_MAX_SPEED_STANDARD)
    {
        I2Cx->CCR.BITS.FS = SET;
        I2Cx->CCR.BITS.DUTY = DutyCycle;
        clockconfig = I2C_SPEED_FAST_TO_CCR(PeriphClock, ClockSpeed, DutyCycle);
    }
    else
    {
        I2Cx->CCR.BITS.FS = RESET;
        clockconfig = I2C_SPEED_STANDARD_TO_CCR(PeriphClock, ClockSpeed);
    }
    
    I2Cx->CCR.REG = (uint16_t) clockconfig;
}
