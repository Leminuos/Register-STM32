#ifndef __HAL_I2C__
#define __HAL_I2C__

#include "stm32f103.h"
#include "stm32_hal_util.h"

#define I2C_MAX_SPEED_STANDARD                  100000U
#define I2C_MAX_SPEED_FAST                      400000U

#define I2C_CR1_PE                              BIT0
#define I2C_CR1_SMBUS                           BIT1
#define I2C_CR1_SMBTYPE                         BIT3
#define I2C_CR1_ENARP                           BIT4
#define I2C_CR1_ENPEC                           BIT5
#define I2C_CR1_ENGC                            BIT6
#define I2C_CR1_NOSTRETCH                       BIT7
#define I2C_CR1_START                           BIT8
#define I2C_CR1_STOP                            BIT9
#define I2C_CR1_ACK                             BIT10
#define I2C_CR1_POS                             BIT11
#define I2C_CR1_PEC                             BIT12
#define I2C_CR1_ALERT                           BIT13
#define I2C_CR1_SWRST                           BIT15

#define I2C_CR2_ITERREN                         BIT8
#define I2C_CR2_ITEVTEN                         BIT9
#define I2C_CR2_ITBUFEN                         BIT10
#define I2C_CR2_DMAEN                           BIT11
#define I2C_CR2_LAST                            BIT12

#define I2C_FLAG_MASK                           0x0000FFFF
#define I2C_FLAG_TIMEOUT                        0x00004000
#define I2C_FLAG_OVR                            0x00000800
#define I2C_FLAG_AF                             0x00000400
#define I2C_FLAG_ARLO                           0x00000200
#define I2C_FLAG_BERR                           0x00000100
#define I2C_FLAG_TXE                            0x00000080
#define I2C_FLAG_RXNE                           0x00000040
#define I2C_FLAG_STOPF                          0x00000010
#define I2C_FLAG_ADD10                          0x00000008
#define I2C_FLAG_BTF                            0x00000004
#define I2C_FLAG_ADDR                           0x00000002
#define I2C_FLAG_SB                             0x00000001
#define I2C_FLAG_DUALF                          0x00010080
#define I2C_FLAG_GENCALL                        0x00010010
#define I2C_FLAG_TRA                            0x00010004
#define I2C_FLAG_BUSY                           0x00010002
#define I2C_FLAG_MSL                            0x00010001

#define I2C_CCR_CCR                             0x00000FFF
#define I2C_DUTYCYCLE                           BIT14

#define CLK_FLAG_I2C(__INSTANCE__, __FLAG__)    ((((uint8_t)((__FLAG__) >> 16)) == 0x01U) ?                                                                     \
                                                (((((__INSTANCE__)->SR2.REG) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) :  \
                                                (((((__INSTANCE__)->SR1.REG) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET))
                                                
#define I2C_ENABLE_PERIPHERAL(__INSTANCE__)     ((__INSTANCE__)->CR1.BITS.PE = SET)
#define I2C_DISABLE_PERIPHERAL(__INSTANCE__)    ((__INSTANCE__)->CR1.BITS.PE = RESET)

#define I2C_ENABLE_START(__INSTANCE__)          ((__INSTANCE__)->CR1.BITS.START = SET)
#define I2C_DISABLE_START(__INSTANCE__)         ((__INSTANCE__)->CR1.BITS.START = RESET)

#define I2C_ENABLE_STOP(__INSTANCE__)           ((__INSTANCE__)->CR1.BITS.STOP = SET)
#define I2C_DISABLE_STOP(__INSTANCE__)          ((__INSTANCE__)->CR1.BITS.STOP = RESET)

#define I2C_GENERATE_ACK(__INSTANCE__)          ((__INSTANCE__)->CR1.BITS.ACK = SET)
#define I2C_GENERATE_NACK(__INSTANCE__)         ((__INSTANCE__)->CR1.BITS.ACK = RESET)

#define I2C_ENABLE_IT_BUFFER(__INSTANCE__)      ((__INSTANCE__)->CR2.BITS.ITBUFEN = SET)
#define I2C_DISABLE_IT_BUFFER(__INSTANCE__)     ((__INSTANCE__)->CR2.BITS.ITBUFEN = RESET)

#define I2C_ENABLE_IT_EVENT(__INSTANCE__)       ((__INSTANCE__)->CR2.BITS.ITEVTEN = SET)
#define I2C_DISABLE_IT_EVENT(__INSTANCE__)      ((__INSTANCE__)->CR2.BITS.ITEVTEN = RESET)

#define I2C_ENABLE_IT_ERROR(__INSTANCE__)       ((__INSTANCE__)->CR2.BITS.ITERREN = SET)
#define I2C_DISABLE_IT_ERROR(__INSTANCE__)      ((__INSTANCE__)->CR2.BITS.ITERREN = RESET)

#define I2C_SEND_DATA(__INSTANCE__, __DATA__)   (((__INSTANCE__)->DR) = ((__DATA__) & 0xFF))
#define I2C_READ_DATA(__INSTANCE__)             ((__INSTANCE__)->DR & 0xFF)

#define I2C_CLEAR_ADDRFLAG(__INSTANCE__)        \
        do {                                    \
            __IO int32_t tempreg = 0;           \
            tempreg = (__INSTANCE__)->SR1.REG;  \
            tempreg = (__INSTANCE__)->SR2.REG;  \
            (void) tempreg;                     \
        } while (0)                             \

#define I2C_PCLK_TO_FREQ(__PCLK__)              ((uint32_t)((__PCLK__) / (1000000UL)))
#define I2C_RISE_TIME(__FREQ__, __SPEED__)      (((__SPEED__) <= (I2C_MAX_SPEED_STANDARD)) ? \
                                                ((__FREQ__) + 1U) :                          \
                                                ((((__FREQ__) * 300U) / 1000U) + 1U))

#define I2C_SPEED_STANDARD_TO_CCR(__PCLK__, __SPEED__)             (uint32_t)(((((__PCLK__)/((__SPEED__) << 1U)) & I2C_CCR_CCR) < 4U)? 4U:((__PCLK__) / ((__SPEED__) << 1U)))
#define I2C_SPEED_FAST_TO_CCR(__PCLK__, __SPEED__, __DUTYCYCLE__)  (uint32_t)(((__DUTYCYCLE__) == I2C_DUTYCYCLE)? \
                                                                   (((((__PCLK__) / ((__SPEED__) * 3U)) & I2C_CCR_CCR) == 0U)? 1U:((__PCLK__) / ((__SPEED__) * 3U))) : \
                                                                   (((((__PCLK__) / ((__SPEED__) * 25U)) & I2C_CCR_CCR) == 0U)? 1U:((__PCLK__) / ((__SPEED__) * 25U))))

void I2C_ConfigSpeed(I2C_Typedef* I2Cx, uint32_t PeriphClock, uint32_t ClockSpeed, uint32_t DutyCycle);

#endif  /* __HAL_I2C__ */
