#ifndef __HAL_UTIL__
#define __HAL_UTIL__

#include <stdint.h>
#include <stddef.h>

typedef enum {
    RESET = 0,
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

typedef enum {
    HAL_STATUS_SUCCESS,
    HAL_STATUS_ERROR,
    HAL_STATUS_BUSY,
    HAL_STATUS_TIMEOUT,
    I2C_STATUS_ERR_START,
    I2C_STATUS_ERR_ADDRESS,
    I2C_STATUS_ERR_TXE,
    I2C_STATUS_ERR_RXNE,
    I2C_STATUS_ERR_BTF,
    I2C_STATUS_ERR_STOP
} RETURN_STATUS;

#define HAL_MAX_TIMEOUT         0xFFFFFFFFU

#define BIT0                    0x00000001
#define BIT1                    0x00000002
#define BIT2                    0x00000004
#define BIT3                    0x00000008
#define BIT4                    0x00000010
#define BIT5                    0x00000020
#define BIT6                    0x00000040
#define BIT7                    0x00000080

#define BIT8                    0x00000100
#define BIT9                    0x00000200
#define BIT10                   0x00000400
#define BIT11                   0x00000800
#define BIT12                   0x00001000
#define BIT13                   0x00002000
#define BIT14                   0x00004000
#define BIT15                   0x00008000

#define BIT16                   0x00010000
#define BIT17                   0x00020000
#define BIT18                   0x00040000
#define BIT19                   0x00080000
#define BIT20                   0x00100000
#define BIT21                   0x00200000
#define BIT22                   0x00400000
#define BIT23                   0x00800000

#define BIT24                   0x01000000
#define BIT25                   0x02000000
#define BIT26                   0x04000000
#define BIT27                   0x08000000
#define BIT28                   0x10000000
#define BIT29                   0x20000000
#define BIT30                   0x40000000
#define BIT31                   0x80000000

typedef union {
    uint32_t word;
    
    struct {
        uint32_t bit0           : 1;
        uint32_t bit1           : 1;
        uint32_t bit2           : 1;
        uint32_t bit3           : 1;
        uint32_t bit4           : 1;
        uint32_t bit5           : 1;
        uint32_t bit6           : 1;
        uint32_t bit7           : 1;
        
        uint32_t bit8           : 1;
        uint32_t bit9           : 1;
        uint32_t bit10          : 1;
        uint32_t bit11          : 1;
        uint32_t bit12          : 1;
        uint32_t bit13          : 1;
        uint32_t bit14          : 1;
        uint32_t bit15          : 1;
        
        uint32_t bit16          : 1;
        uint32_t bit17          : 1;
        uint32_t bit18          : 1;
        uint32_t bit19          : 1;
        uint32_t bit20          : 1;
        uint32_t bit21          : 1;
        uint32_t bit22          : 1;
        uint32_t bit23          : 1;
        
        uint32_t bit24          : 1;
        uint32_t bit25          : 1;
        uint32_t bit26          : 1;
        uint32_t bit27          : 1;
        uint32_t bit28          : 1;
        uint32_t bit29          : 1;
        uint32_t bit30          : 1;
        uint32_t bit31          : 1;
    } bitfield;
} BITS;

#define SET_BIT(REG, BIT)       ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)     ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)      ((((REG) & (BIT)) == (BIT)) ? (SET) : (RESET))

#define TOGGLE_BIT(__BIT__)     ((__BIT__) = (!(__BIT__)))

#define CLEAR_REG(REG)          (REG = (0x0))

#define WRITE_REG(REG, VAL)     ((REG) = (VAL))

#define READ_REG(REG)           ((REG))

#define MODIFY_REG(REG, CLEAR_BIT, MASK_BIT)    WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
#ifndef __ALIGN_END
#define __ALIGN_END    __attribute__ ((aligned (4)))
#endif /* __ALIGN_END */
#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif /* __ALIGN_BEGIN */
#else
#ifndef __ALIGN_END
#define __ALIGN_END
#endif /* __ALIGN_END */
#ifndef __ALIGN_BEGIN
#if defined   (__CC_ARM)      /* ARM Compiler */
#define __ALIGN_BEGIN    __align(4)
#elif defined (__ICCARM__)    /* IAR Compiler */
#define __ALIGN_BEGIN
#endif /* __CC_ARM */
#endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */

#endif  /* __HAL_UTIL__ */
