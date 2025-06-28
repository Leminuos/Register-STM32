#ifndef __HAL_I2C__
#define __HAL_I2C__

#include "stm32f103.h"
#include "stm32_hal_util.h"

typedef enum {
    I2C_FLAG_TIMEOUT    =   0x00004000,
    I2C_FLAG_OVR        =   0x00000800,
    I2C_FLAG_AF         =   0x00000400,
    I2C_FLAG_ARLO       =   0x00000200,
    I2C_FLAG_BERR       =   0x00000100,
    I2C_FLAG_TXE        =   0x00000080,
    I2C_FLAG_RXNE       =   0x00000040,
    I2C_FLAG_STOPF      =   0x00000010,
    I2C_FLAG_ADD10      =   0x00000008,
    I2C_FLAG_BTF        =   0x00000004,
    I2C_FLAG_ADDR       =   0x00000002,
    I2C_FLAG_SB         =   0x00000001,
    I2C_FLAG_DUALF      =   0x00010080,
    I2C_FLAG_GENCALL    =   0x00010010,
    I2C_FLAG_TRA        =   0x00010004,
    I2C_FLAG_BUSY       =   0x00010002,
    I2C_FLAG_MSL        =   0x00010001
} I2C_STATUS_FLAG;

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

#define I2C_CCR_CCR                             0x00000FFF
#define I2C_DUTYCYCLE                           BIT14

#define I2C_FLAG_MASK                           0x0000FFFF
#define I2C_CHK_FLAG(__INSTANCE__, __FLAG__)    ((((uint8_t)((__FLAG__) >> 16)) == 0x01U) ?                                                                     \
                                                (((((__INSTANCE__)->SR2.REG) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) :  \
                                                (((((__INSTANCE__)->SR1.REG) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET))
                                                
#define I2C_ENABLE_PERIPHERAL(__INSTANCE__)     ((__INSTANCE__)->CR1.BITS.PE = SET)
#define I2C_DISABLE_PERIPHERAL(__INSTANCE__)    ((__INSTANCE__)->CR1.BITS.PE = RESET)
#define I2C_CHECK_PERIPHERAL(__INSTANCE__)      ((__INSTANCE__)->CR1.BITS.PE)

#define I2C_ENABLE_START(__INSTANCE__)          ((__INSTANCE__)->CR1.BITS.START = SET)
#define I2C_DISABLE_START(__INSTANCE__)         ((__INSTANCE__)->CR1.BITS.START = RESET)

#define I2C_ENABLE_STOP(__INSTANCE__)           ((__INSTANCE__)->CR1.BITS.STOP = SET)
#define I2C_DISABLE_STOP(__INSTANCE__)          ((__INSTANCE__)->CR1.BITS.STOP = RESET)

#define I2C_ENABLE_POS(__INSTANCE__)            ((__INSTANCE__)->CR1.BITS.POS = SET)
#define I2C_DISABLE_POS(__INSTANCE__)           ((__INSTANCE__)->CR1.BITS.POS = RESET)

#define I2C_GENERATE_ACK(__INSTANCE__)          ((__INSTANCE__)->CR1.BITS.ACK = SET)
#define I2C_GENERATE_NACK(__INSTANCE__)         ((__INSTANCE__)->CR1.BITS.ACK = RESET)

#define I2C_NO_STRETCH(__INSTANCE__, __VAL__)   ((__INSTANCE__)->CR1.BITS.NOSTRETCH = (__VAL__))

#define I2C_GENERAL_CALL(__INSTANCE__, __VAL__) ((__INSTANCE__)->CR1.BITS.ENGC = (__VAL__))

#define I2C_ADDR_MODE(__INSTANCE__, __VAL__)    ((__INSTANCE__)->OAR1.BITS.ADDMODE = (__VAL__))

#define I2C_OWN_ADDR_1(__INSTANCE__, __VAL__)   ((__INSTANCE__)->OAR1.REG = (((__VAL__) << 1U) | (1U << 14U)))

#define I2C_OWN_ADDR_2(__INSTANCE__, __VAL__)   ((__INSTANCE__)->OAR2.BITS.ADD2 = (__VAL__))

#define I2C_ENABLE_IT_BUFFER(__INSTANCE__)      ((__INSTANCE__)->CR2.BITS.ITBUFEN = (SET))
#define I2C_DISABLE_IT_BUFFER(__INSTANCE__)     ((__INSTANCE__)->CR2.BITS.ITBUFEN = (RESET))

#define I2C_ENABLE_IT_EVENT(__INSTANCE__)       ((__INSTANCE__)->CR2.BITS.ITEVTEN = (SET))
#define I2C_DISABLE_IT_EVENT(__INSTANCE__)      ((__INSTANCE__)->CR2.BITS.ITEVTEN = (RESET))

#define I2C_ENABLE_IT_ERROR(__INSTANCE__)       ((__INSTANCE__)->CR2.BITS.ITERREN = (SET))
#define I2C_DISABLE_IT_ERROR(__INSTANCE__)      ((__INSTANCE__)->CR2.BITS.ITERREN = (RESET))

#define I2C_SEND_DATA(__INSTANCE__, __VAL__)    (((__INSTANCE__)->DR) = ((__VAL__) & 0xFF))
#define I2C_READ_DATA(__INSTANCE__)             ((__INSTANCE__)->DR & 0xFF)

#define I2C_IT_BUFFER                           0x00000400U
#define I2C_IT_EVENT                            0x00000200U
#define I2C_IT_ERROR                            0x00000100U

#define I2C_ENABLE_IT(__INSTANCE__, __FLAG__)   ((__INSTANCE__)->CR2.REG |= (__FLAG__))
#define I2C_CLEAR_IT(__INSTANCE__, __FLAG__)    ((__INSTANCE__)->CR2.REG &= (~(__FLAG__)))

#define I2C_CHK_IT(__INSTANCE__, __FLAG__)      ((((__INSTANCE__)->CR2.REG & (__FLAG__)) == (__FLAG__)) ? (SET) : (RESET))

#define I2C_CLEAR_AF(__INSTANCE__)              ((__INSTANCE__)->SR1.BITS.AF = RESET)

#define I2C_SOFTWARE_RESET(__INSTANCE__)            \
        do {                                        \
            (__INSTANCE__)->CR1.BITS.SWRST = SET;   \
            (__INSTANCE__)->CR1.BITS.SWRST = RESET; \
        } while (0)                                 \

#define I2C_CLEAR_ADDRFLAG(__INSTANCE__)        \
        do {                                    \
            __IO int32_t tempreg = 0;           \
            tempreg = (__INSTANCE__)->SR1.REG;  \
            tempreg = (__INSTANCE__)->SR2.REG;  \
            (void) tempreg;                     \
        } while (0)                             \

#define I2C_CLEAR_STOPF(__INSTANCE__)           \
        do {                                    \
            __IO int32_t tempreg = 0;           \
            tempreg = (__INSTANCE__)->SR1.REG;  \
            tempreg = (__INSTANCE__)->CR1.REG;  \
            tempreg &= ~(1 << 9)             ;  \
            (__INSTANCE__)->CR1.REG = tempreg;  \
        } while (0)                             \

#define I2C_CLEAR_PENDING_IRQ(__INSTANCE__)                 \
        do {                                                                \
            __IO I2C_Typedef *tempointer = (I2C_Typedef *)(__INSTANCE__);   \
                                                                            \
            if (I2C1 == tempointer)                                         \
            {                                                               \
                NVIC_ClearPendingIRQ(I2C1_EV_IRQn);                         \
            }                                                               \
            else if (I2C2 == tempointer)                                    \
            {                                                               \
                NVIC_ClearPendingIRQ(I2C2_EV_IRQn);                         \
            }                                                               \
        } while(0)                                                          \

#define I2C_PCLK_TO_FREQ(__PCLK__)              ((uint32_t)((__PCLK__) / (1000000UL)))
#define I2C_RISE_TIME(__FREQ__, __SPEED__)      (((__SPEED__) <= (I2C_MAX_SPEED_STANDARD)) ? \
                                                ((__FREQ__) + 1U) :                          \
                                                ((((__FREQ__) * 300U) / 1000U) + 1U))

#define I2C_SPEED_STANDARD_TO_CCR(__PCLK__, __SPEED__)             (uint32_t)(((((__PCLK__)/((__SPEED__) << 1U)) & I2C_CCR_CCR) < 4U)? 4U:((__PCLK__) / ((__SPEED__) << 1U)))
#define I2C_SPEED_FAST_TO_CCR(__PCLK__, __SPEED__, __DUTYCYCLE__)  (uint32_t)(((__DUTYCYCLE__) == I2C_DUTYCYCLE)? \
                                                                   (((((__PCLK__) / ((__SPEED__) * 3U)) & I2C_CCR_CCR) == 0U)? 1U:((__PCLK__) / ((__SPEED__) * 3U))) : \
                                                                   (((((__PCLK__) / ((__SPEED__) * 25U)) & I2C_CCR_CCR) == 0U)? 1U:((__PCLK__) / ((__SPEED__) * 25U))))

extern uint32_t GetCounterTick(void);
void I2C_ConfigSpeed(I2C_Typedef* I2Cx, uint32_t PeriphClock, uint32_t ClockSpeed, uint32_t DutyCycle);
RETURN_STATUS I2C_WaitOnFlagUntilTimeout(I2C_Typedef* I2Cx, uint32_t Timeout, uint32_t TickStart, I2C_STATUS_FLAG StatusFlag);
RETURN_STATUS I2C_WaitBusyUntilTimeout(I2C_Typedef* I2Cx, uint32_t Timeout, uint32_t TickStart);

#endif  /* __HAL_I2C__ */
