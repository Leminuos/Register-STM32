#ifndef __STM32F103_
#define __STM32F103_

#define __NVIC_PRIO_BITS          4       /*!< STM32 uses 4 Bits for the Priority Levels    */

typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

/******  STM32 specific Interrupt Numbers *********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
  TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
  RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */
  ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
  USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
  USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
  TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
  TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
  TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
  RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
  USBWakeUp_IRQn              = 42      /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
} IRQn_Type;

#include <core_cm3.h>

typedef struct {
    __IO union {
        uint32_t REG;                   /* Reset value: : 0x0000XX83 where X is undefined. */

        struct {
            uint32_t HSION      : 1;    /* Internal high-speed clock enable */
            uint32_t HSIRDY     : 1;    /* Internal high-speed clock ready flag */
            uint32_t reserved0  : 1;
            uint32_t HSITRIM    : 5;    /* Internal high-speed clock trimming */
            uint32_t HSICAL     : 8;    /* Internal high-speed clock calibration */
            uint32_t HSEON      : 1;    /* HSE clock enable */
            uint32_t HSERDY     : 1;    /* External high-speed clock ready flag */
            uint32_t HSEBYP     : 1;    /* External high-speed clock bypass */
            uint32_t CSSON      : 1;    /* Clock security system enable */
            uint32_t reserved1  : 4;
            uint32_t PLLON      : 1;    /* PLL enable */
            uint32_t PLLRDY     : 1;    /* PLL clock ready flag */
            uint32_t reserved2  : 6;
        } BITS;
    } CR;                               /* Clock control register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t SW         : 2;    /* System clock switch */
            uint32_t SWS        : 2;    /* System clock switch status */
            uint32_t HPRE       : 4;    /* AHB prescaler */
            uint32_t PPRE1      : 3;    /* APB low-speed prescaler (APB1) */
            uint32_t PPRE2      : 3;    /* APB high-speed prescaler (APB2) */
            uint32_t ADCPRE     : 2;    /* ADC prescaler */
            uint32_t PLLSRC     : 1;    /* PLL entry clock source */
            uint32_t PLLXTPRE   : 1;    /* HSE divider for PLL entry */
            uint32_t PLLMUL     : 4;    /* PLL multiplication factor */
            uint32_t USBPRE     : 1;    /* USB prescaler */
            uint32_t reserved0  : 1;
            uint32_t MCO        : 3;    /* Microcontroller clock output */
            uint32_t reserved1  : 5;
        } BITS;
    } CFGR;                             /* Clock configuration register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t LSIRDYF    : 1;    /* LSI ready interrupt flag */
            uint32_t LSERDYF    : 1;    /* LSE ready interrupt flag */
            uint32_t HSIRDYF    : 1;    /* HSI ready interrupt flag */
            uint32_t HSERDYF    : 1;    /* HSE ready interrupt flag */
            uint32_t PLLRDYF    : 1;    /* PLL ready interrupt flag */
            uint32_t reserved0  : 2;
            uint32_t CSSF       : 1;    /* Clock security system interrupt flag */
            uint32_t LSIRDYIE   : 1;    /* LSI ready interrupt enable */
            uint32_t LSERDYIE   : 1;    /* LSE ready interrupt enable */
            uint32_t HSIRDYIE   : 1;    /* HSI ready interrupt enable */
            uint32_t HSERDYIE   : 1;    /* HSE ready interrupt enable */
            uint32_t PLLRDYIE   : 1;    /* PLL ready interrupt enable */
            uint32_t reserved1  : 2;
            uint32_t LSIRDYC    : 1;    /* LSI ready interrupt clear */
            uint32_t LSERDYC    : 1;    /* LSE ready interrupt clear */
            uint32_t HSIRDYC    : 1;    /* HSI ready interrupt clear */
            uint32_t HSERDYC    : 1;    /* HSE ready interrupt clear */
            uint32_t PLLRDYC    : 1;    /* PLL ready interrupt clear */
            uint32_t CSSC       : 1;    /* Clock security system interrupt clear */
            uint32_t reserved2  : 8;
        } BITS;
    } CIR;                              /* Clock interrupt register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t AFIORST    : 1;    /* Alternate function IO reset */
            uint32_t reserved0  : 1;
            uint32_t IOPARST    : 1;    /* IO port A reset */
            uint32_t IOPBRST    : 1;    /* IO port B reset */
            uint32_t IOPCRST    : 1;    /* IO port C reset */
            uint32_t IOPDRST    : 1;    /* IO port D reset */
            uint32_t IOPERST    : 1;    /* IO port E reset */
            uint32_t IOPFRST    : 1;    /* IO port F reset */
            uint32_t IOPGRST    : 1;    /* IO port G reset */
            uint32_t ADC1RST    : 1;    /* ADC 1 interface reset */
            uint32_t ADC2RST    : 1;    /* ADC 2 interface reset */
            uint32_t TIM1RST    : 1;    /* TIM1 timer rese */
            uint32_t SPI1RST    : 1;    /* SPI1 reset */
            uint32_t TIM8RST    : 1;    /* TIM8 timer reset */
            uint32_t USART1RST  : 1;    /* USART1 reset */
            uint32_t reserved1  : 3;
            uint32_t TIM9RST    : 1;    /* TIM8 timer reset */
            uint32_t TIM10RST   : 1;    /* TIM10 timer reset */
            uint32_t TIM11RST   : 1;    /* TIM11 timer reset */
            uint32_t reserved2  : 10;
        } BITS;
    } APB2RSTR;                         /* APB2 peripheral reset register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t TIM2RST    : 1;    /* TIM2 timer reset */
            uint32_t TIM3RST    : 1;    /* TIM3 timer reset */
            uint32_t TIM4RST    : 1;    /* TIM4 timer reset */
            uint32_t TIM5RST    : 1;    /* TIM5 timer reset */
            uint32_t TIM6RST    : 1;    /* TIM6 timer reset */
            uint32_t TIM7RST    : 1;    /* TIM7 timer reset */
            uint32_t TIM12RST   : 1;    /* TIM12 timer reset */
            uint32_t TIM13RST   : 1;    /* TIM13 timer reset */
            uint32_t TIM14RST   : 1;    /* TIM14 timer reset */
            uint32_t reserved0  : 2;
            uint32_t WWDGRST    : 1;    /* Window watchdog reset */
            uint32_t SPI2RST    : 1;    /* SPI2 reset */
            uint32_t SPI3RST    : 1;    /* SPI3 reset */
            uint32_t reserved1  : 1;
            uint32_t USART2RST  : 1;    /* USART2 reset */
            uint32_t USART3RST  : 1;    /* USART3 reset */
            uint32_t USART4RST  : 1;    /* USART4 reset */
            uint32_t USART5RST  : 1;    /* USART5 reset */
            uint32_t I2C1RST    : 1;    /* I2C1 reset */
            uint32_t I2C2RST    : 1;    /* I2C2 reset */
            uint32_t USBRST     : 1;    /* USBRST reset */
            uint32_t reserved2  : 1;
            uint32_t CANRST     : 1;    /* CANRST reset */
            uint32_t reserved3  : 1;
            uint32_t BKPRST     : 1;    /* Backup interface reset */
            uint32_t PWRRST     : 1;    /* Power interface reset */
            uint32_t DACRST     : 1;    /* DAC interface reset */
            uint32_t reserved4  : 1;
        } BITS;
    } APB1RSTR;                         /* APB1 peripheral reset register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000014 */

        struct {
            uint32_t DMA1EN     : 1;    /* DMA1 clock enable */
            uint32_t DMA2EN     : 1;    /* DMA2 clock enable */
            uint32_t SRAMEN     : 1;    /* SRAM interface clock enable */
            uint32_t reserved0  : 1;
            uint32_t FLITFEN    : 1;    /* FLITF clock enable */
            uint32_t reserved1  : 1;
            uint32_t CRCEN      : 1;    /* CRC clock enable */
            uint32_t reserved2  : 1;
            uint32_t FSMCEN     : 1;    /* FSMC clock enable */
            uint32_t reserved3  : 1;
            uint32_t SDIOEN     : 1;    /* SDIO clock enable */
            uint32_t reserved4  : 21;
        } BITS;
    } AHBENR;                           /* AHB peripheral clock enable register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t AFIOEN     : 1;    /* Alternate function IO clock enable */
            uint32_t reserved0  : 1;
            uint32_t IOPAEN     : 1;    /* IO port A clock enable */
            uint32_t IOPBEN     : 1;    /* IO port B clock enable */
            uint32_t IOPCEN     : 1;    /* IO port C clock enable */
            uint32_t IOPDEN     : 1;    /* IO port D clock enable */
            uint32_t IOPEEN     : 1;    /* IO port E clock enable */
            uint32_t IOPFEN     : 1;    /* IO port F clock enable */
            uint32_t IOPGEN     : 1;    /* IO port G clock enable */
            uint32_t ADC1EN     : 1;    /* ADC 1 interface clock enable */
            uint32_t ADC2EN     : 1;    /* ADC 2 interface clock enable */
            uint32_t TIM1EN     : 1;    /* TIM1 timer clock enable */
            uint32_t SPI1EN     : 1;    /* SPI1 clock enable */
            uint32_t TIM8EN     : 1;    /* TIM8 Timer clock enable */
            uint32_t USART1EN   : 1;    /* USART1 clock enable */
            uint32_t ADC3EN     : 1;    /* ADC3 interface clock enable */
            uint32_t reserved1  : 3;
            uint32_t TIM9EN     : 1;    /* TIM9 Timer clock enable */
            uint32_t TIM10EN    : 1;    /* TIM10 Timer clock enable */
            uint32_t TIM11EN    : 1;    /* TIM11 Timer clock enable */
            uint32_t reserved2  : 10;
        } BITS;
    } APB2ENR;                          /* APB2 peripheral clock enable register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t TIM2EN     : 1;    /* TIM2 timer clock enable */
            uint32_t TIM3EN     : 1;    /* TIM3 timer clock enable */
            uint32_t TIM4EN     : 1;    /* TIM4 timer clock enable */
            uint32_t TIM5EN     : 1;    /* TIM5 timer clock enable */
            uint32_t TIM6EN     : 1;    /* TIM6 timer clock enable */
            uint32_t TIM7EN     : 1;    /* TIM7 timer clock enable */
            uint32_t TIM12EN    : 1;    /* TIM12 timer clock enable */
            uint32_t TIM13EN    : 1;    /* TIM13 timer clock enable */
            uint32_t TIM14EN    : 1;    /* TIM14 timer clock enable */
            uint32_t reserved0  : 2;
            uint32_t WWDGEN     : 1;    /* Window watchdog clock enable */
            uint32_t reserved1  : 2;
            uint32_t SPI2EN     : 1;    /* SPI2 clock enable */
            uint32_t SPI3EN     : 1;    /* SPI3 clock enable */
            uint32_t reserved2  : 1;
            uint32_t USART2EN   : 1;    /* USART2 clock enable */
            uint32_t USART3EN   : 1;    /* USART3 clock enable */
            uint32_t USART4EN   : 1;    /* USART4 clock enable */
            uint32_t USART5EN   : 1;    /* USART5 clock enable */
            uint32_t I2C1EN     : 1;    /* I2C1 clock enable */
            uint32_t I2C2EN     : 1;    /* I2C2 clock enable */
            uint32_t USBEN      : 1;    /* USBEN clock enable */
            uint32_t reserved3  : 1;
            uint32_t CANEN      : 1;    /* CANEN clock enable */
            uint32_t reserved4  : 1;
            uint32_t BKPEN      : 1;    /* Backup interface clock enable */
            uint32_t PWREN      : 1;    /* Power interface clock enable */
            uint32_t DACEN      : 1;    /* DAC interface clock enable */
            uint32_t reserved5  : 2;
        } BITS;
    } APB1ENR;                          /* APB1 peripheral clock enable register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t LSEON      : 1;    /* External low-speed oscillator enable */
            uint32_t LSERDY     : 1;    /* External low-speed oscillator ready */
            uint32_t LSEBYP     : 1;    /* External low-speed oscillator bypass */
            uint32_t reserved0  : 5;
            uint32_t RTCSEL     : 2;    /* RTC clock source selection */
            uint32_t reserved1  : 5;
            uint32_t RTCEN      : 1;    /* RTC clock enable */
            uint32_t BDRST      : 1;    /* Backup domain software reset */
            uint32_t reserved2  : 15;
        } BITS;
    } BDCR;                             /* Backup domain control register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x0C000000 */

        struct {
            uint32_t LSION      : 1;    /* Internal low-speed oscillator enable */
            uint32_t LSIRDY     : 1;    /* Ixternal low-speed oscillator ready */
            uint32_t reserved0  : 22;
            uint32_t RMVF       : 1;    /* Remove reset flag */
            uint32_t reserved1  : 1;
            uint32_t PINRSTF    : 1;    /* PIN reset flag */
            uint32_t PORRSTF    : 1;    /* POR/PDR reset flag */
            uint32_t SFTRSTF    : 1;    /* Software reset flag */
            uint32_t IWDGRSTF   : 1;    /* Independent watchdog reset flag */
            uint32_t WWDGRSTF   : 1;    /* Window watchdog reset flag */
            uint32_t LPWRRSTF   : 1;    /* Low-power reset flag */
        } BITS;
    } CSR;                              /* Control/status register */
} RCC_Typedef;

typedef struct {
    union {
        uint32_t REG;
        
        struct {
            uint32_t MODE0      : 2;
            uint32_t CNF0       : 2;
            uint32_t MODE1      : 2;
            uint32_t CNF1       : 2;
            uint32_t MODE2      : 2;
            uint32_t CNF2       : 2;
            uint32_t MODE3      : 2;
            uint32_t CNF3       : 2;
            uint32_t MODE4      : 2;
            uint32_t CNF4       : 2;
            uint32_t MODE5      : 2;
            uint32_t CNF5       : 2;
            uint32_t MODE6      : 2;
            uint32_t CNF6       : 2;
            uint32_t MODE7      : 2;
            uint32_t CNF7       : 2;
        } BITS;
    } CRL;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t MODE8      : 2;
            uint32_t CNF8       : 2;
            uint32_t MODE9      : 2;
            uint32_t CNF9       : 2;
            uint32_t MODE10     : 2;
            uint32_t CNF10      : 2;
            uint32_t MODE11     : 2;
            uint32_t CNF11      : 2;
            uint32_t MODE12     : 2;
            uint32_t CNF12      : 2;
            uint32_t MODE13     : 2;
            uint32_t CNF13      : 2;
            uint32_t MODE14     : 2;
            uint32_t CNF14      : 2;
            uint32_t MODE15     : 2;
            uint32_t CNF15      : 2;
        } BITS;
    } CRH;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t IDR0       : 1;
            uint32_t IDR1       : 1;
            uint32_t IDR2       : 1;
            uint32_t IDR3       : 1;
            uint32_t IDR4       : 1;
            uint32_t IDR5       : 1;
            uint32_t IDR6       : 1;
            uint32_t IDR7       : 1;
            uint32_t IDR8       : 1;
            uint32_t IDR9       : 1;
            uint32_t IDR10      : 1;
            uint32_t IDR11      : 1;
            uint32_t IDR12      : 1;
            uint32_t IDR13      : 1;
            uint32_t IDR14      : 1;
            uint32_t IDR15      : 1;
            uint32_t reversed   : 16;
        } BITS;
    } IDR;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t ODR0       : 1;
            uint32_t ODR1       : 1;
            uint32_t ODR2       : 1;
            uint32_t ODR3       : 1;
            uint32_t ODR4       : 1;
            uint32_t ODR5       : 1;
            uint32_t ODR6       : 1;
            uint32_t ODR7       : 1;
            uint32_t ODR8       : 1;
            uint32_t ODR9       : 1;
            uint32_t ODR10      : 1;
            uint32_t ODR11      : 1;
            uint32_t ODR12      : 1;
            uint32_t ODR13      : 1;
            uint32_t ODR14      : 1;
            uint32_t ODR15      : 1;
            uint32_t reversed   : 16;
        } BITS;
    } ODR;
 
    union {
        uint32_t REG;
        
        struct {
            uint32_t BS0        : 1;
            uint32_t BS1        : 1;
            uint32_t BS2        : 1;
            uint32_t BS3        : 1;
            uint32_t BS4        : 1;
            uint32_t BS5        : 1;
            uint32_t BS6        : 1;
            uint32_t BS7        : 1;
            uint32_t BS8        : 1;
            uint32_t BS9        : 1;
            uint32_t BS10       : 1;
            uint32_t BS11       : 1;
            uint32_t BS12       : 1;
            uint32_t BS13       : 1;
            uint32_t BS14       : 1;
            uint32_t BS15       : 1;
            uint32_t BR0        : 1;
            uint32_t BR1        : 1;
            uint32_t BR2        : 1;
            uint32_t BR3        : 1;
            uint32_t BR4        : 1;
            uint32_t BR5        : 1;
            uint32_t BR6        : 1;
            uint32_t BR7        : 1;
            uint32_t BR8        : 1;
            uint32_t BR9        : 1;
            uint32_t BR10       : 1;
            uint32_t BR11       : 1;
            uint32_t BR12       : 1;
            uint32_t BR13       : 1;
            uint32_t BR14       : 1;
            uint32_t BR15       : 1;
        } BITS;
    } BSRR;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t BR0        : 1;
            uint32_t BR1        : 1;
            uint32_t BR2        : 1;
            uint32_t BR3        : 1;
            uint32_t BR4        : 1;
            uint32_t BR5        : 1;
            uint32_t BR6        : 1;
            uint32_t BR7        : 1;
            uint32_t BR8        : 1;
            uint32_t BR9        : 1;
            uint32_t BR10       : 1;
            uint32_t BR11       : 1;
            uint32_t BR12       : 1;
            uint32_t BR13       : 1;
            uint32_t BR14       : 1;
            uint32_t BR15       : 1;
            uint32_t reversed   : 16;
        } BITS;
    } BRR;

    union {
        uint32_t REG;
        
        struct {
            uint32_t LCK0        : 1;
            uint32_t LCK1        : 1;
            uint32_t LCK2        : 1;
            uint32_t LCK3        : 1;
            uint32_t LCK4        : 1;
            uint32_t LCK5        : 1;
            uint32_t LCK6        : 1;
            uint32_t LCK7        : 1;
            uint32_t LCK8        : 1;
            uint32_t LCK9        : 1;
            uint32_t LCK10       : 1;
            uint32_t LCK11       : 1;
            uint32_t LCK12       : 1;
            uint32_t LCK13       : 1;
            uint32_t LCK14       : 1;
            uint32_t LCK15       : 1;
            uint32_t LCKK        : 1;
            uint32_t reversed    : 15;
        } BITS;
    } LCKR;
} GPIO_Typedef;

typedef struct {
    union {
        uint32_t REG;
        
        struct {
            uint32_t PIN         : 4;       /* Pin selection        */
                                            /* 0000: Px0 selected   */
                                            /* 0001: Px1 selected   */
                                            /* 0010: Px2 selected   */
                                            /* 0011: Px3 selected   */
                                            /* ...                  */
                                            /* 1111: Px15 selected  */
            
            uint32_t PORT        : 3;       /* Port selection       */
                                            /* 000: PA selected     */
                                            /* 001: PB selected     */
                                            /* 010: PC selected     */
                                            /* 011: PD selected     */
                                            /* 100: PE selected     */
            
            uint32_t EVOE        : 1;       /* Event output enable  */
            uint32_t reversed    : 24;
        } BITS;
    } EVCR;         /* Event control register */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t SPI1_REMAP         : 1;
            uint32_t I2C1_REMAP         : 1;
            uint32_t USART1_REMAP       : 1;
            uint32_t USART2_REMAP       : 1;
            uint32_t USART3_REMAP       : 2;
            uint32_t TIM1_REMAP         : 2;
            uint32_t TIM2_REMAP         : 2;
            uint32_t TIM3_REMAP         : 2;
            uint32_t TIM4_REMAP         : 1;
            uint32_t CAN1_REMAP         : 2;
            uint32_t PD01_REMAP         : 1;
            uint32_t TIM5CH4_IREMAP     : 1;
            uint32_t ADC1_ETRGINJ_REMAP : 1;
            uint32_t ADC1_ETRGREG_REMAP : 1;
            uint32_t ADC2_ETRGINJ_REMAP : 1;
            uint32_t ADC2_ETRGREG_REMAP : 1;
            uint32_t reversed1          : 3;
            uint32_t SWJ_CFG            : 3;
            uint32_t reversed2          : 5;
        } BITS;
    } MAPR;         /* AF remap and debug I/O configuration register */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI0       : 4;       /* EXTIx configuration (x = 0 to 3) */
                                            /* 0000: PA[x] pin                  */
                                            /* 0001: PB[x] pin                  */
                                            /* 0010: PC[x] pin                  */
                                            /* 0011: PD[x] pin                  */
                                            /* 0100: PE[x] pin                  */
                                            /* 0101: PF[x] pin                  */
                                            /* 0110: PG[x] pin                  */
            uint32_t EXTI1       : 4;
            uint32_t EXTI2       : 4;
            uint32_t EXTI3       : 4;
            uint32_t reversed    : 16;
        } BITS;     /* External interrupt configuration register 1 */
    } EXTICR1;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI4       : 4;       /* EXTI x configuration (x = 0 to 3)*/
            uint32_t EXTI5       : 4;
            uint32_t EXTI6       : 4;
            uint32_t EXTI7       : 4;
            uint32_t reversed    : 16;
        } BITS;     /* External interrupt configuration register 2 */
    } EXTICR2;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI8       : 4;       /* EXTI x configuration (x = 0 to 3)*/
            uint32_t EXTI9       : 4;
            uint32_t EXTI10      : 4;
            uint32_t EXTI11      : 4;
            uint32_t reversed    : 16;
        } BITS;     /* External interrupt configuration register 3 */
    } EXTICR3;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI12      : 4;       /* EXTI x configuration (x = 0 to 3)*/
            uint32_t EXTI13      : 4;
            uint32_t EXTI14      : 4;
            uint32_t EXTI15      : 4;
            uint32_t reversed    : 16;
        } BITS;     /* External interrupt configuration register 3 */
    } EXTICR4;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t reversed1   : 5;
            uint32_t TIM9_REMAP  : 1;
            uint32_t TIM10_REMAP : 1;
            uint32_t TIM11_REMAP : 1;
            uint32_t TIM13_REMAP : 1;
            uint32_t TIM14_REMAP : 1;
            uint32_t FSMC_NADV   : 1;
            uint32_t reversed2   : 21;
        } BITS;
    } MAPR2;        /* AF remap and debug I/O configuration register2 */
} AFIO_Typedef;

typedef struct {
    union {
        uint32_t REG;
        
        struct {
            uint32_t MR0         : 1;
            uint32_t MR1         : 1;
            uint32_t MR2         : 1;
            uint32_t MR3         : 1;
            uint32_t MR4         : 1;
            uint32_t MR5         : 1;
            uint32_t MR6         : 1;
            uint32_t MR7         : 1;
            uint32_t MR8         : 1;
            uint32_t MR9         : 1;
            uint32_t MR10        : 1;
            uint32_t MR11        : 1;
            uint32_t MR12        : 1;
            uint32_t MR13        : 1;
            uint32_t MR14        : 1;
            uint32_t MR15        : 1;
            uint32_t MR16        : 1;
            uint32_t MR17        : 1;
            uint32_t MR18        : 1;
            uint32_t MR19        : 1;
            uint32_t reversed    : 12;
        } BITS;
    } IMR;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t MR0         : 1;
            uint32_t MR1         : 1;
            uint32_t MR2         : 1;
            uint32_t MR3         : 1;
            uint32_t MR4         : 1;
            uint32_t MR5         : 1;
            uint32_t MR6         : 1;
            uint32_t MR7         : 1;
            uint32_t MR8         : 1;
            uint32_t MR9         : 1;
            uint32_t MR10        : 1;
            uint32_t MR11        : 1;
            uint32_t MR12        : 1;
            uint32_t MR13        : 1;
            uint32_t MR14        : 1;
            uint32_t MR15        : 1;
            uint32_t MR16        : 1;
            uint32_t MR17        : 1;
            uint32_t MR18        : 1;
            uint32_t MR19        : 1;
            uint32_t reversed    : 12;
        } BITS;
    } EMR;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t TR0         : 1;
            uint32_t TR1         : 1;
            uint32_t TR2         : 1;
            uint32_t TR3         : 1;
            uint32_t TR4         : 1;
            uint32_t TR5         : 1;
            uint32_t TR6         : 1;
            uint32_t TR7         : 1;
            uint32_t TR8         : 1;
            uint32_t TR9         : 1;
            uint32_t TR10        : 1;
            uint32_t TR11        : 1;
            uint32_t TR12        : 1;
            uint32_t TR13        : 1;
            uint32_t TR14        : 1;
            uint32_t TR15        : 1;
            uint32_t TR16        : 1;
            uint32_t TR17        : 1;
            uint32_t TR18        : 1;
            uint32_t TR19        : 1;
            uint32_t reversed    : 12;
        } BITS;
    } RTSR;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t TR0         : 1;
            uint32_t TR1         : 1;
            uint32_t TR2         : 1;
            uint32_t TR3         : 1;
            uint32_t TR4         : 1;
            uint32_t TR5         : 1;
            uint32_t TR6         : 1;
            uint32_t TR7         : 1;
            uint32_t TR8         : 1;
            uint32_t TR9         : 1;
            uint32_t TR10        : 1;
            uint32_t TR11        : 1;
            uint32_t TR12        : 1;
            uint32_t TR13        : 1;
            uint32_t TR14        : 1;
            uint32_t TR15        : 1;
            uint32_t TR16        : 1;
            uint32_t TR17        : 1;
            uint32_t TR18        : 1;
            uint32_t TR19        : 1;
            uint32_t reversed    : 12;
        } BITS;
    } FTSR;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t SWIER0         : 1;
            uint32_t SWIER1         : 1;
            uint32_t SWIER2         : 1;
            uint32_t SWIER3         : 1;
            uint32_t SWIER4         : 1;
            uint32_t SWIER5         : 1;
            uint32_t SWIER6         : 1;
            uint32_t SWIER7         : 1;
            uint32_t SWIER8         : 1;
            uint32_t SWIER9         : 1;
            uint32_t SWIER10        : 1;
            uint32_t SWIER11        : 1;
            uint32_t SWIER12        : 1;
            uint32_t SWIER13        : 1;
            uint32_t SWIER14        : 1;
            uint32_t SWIER15        : 1;
            uint32_t SWIER16        : 1;
            uint32_t SWIER17        : 1;
            uint32_t SWIER18        : 1;
            uint32_t SWIER19        : 1;
            uint32_t reversed    : 12;
        } BITS;
    } SWIER;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t PR0         : 1;
            uint32_t PR1         : 1;
            uint32_t PR2         : 1;
            uint32_t PR3         : 1;
            uint32_t PR4         : 1;
            uint32_t PR5         : 1;
            uint32_t PR6         : 1;
            uint32_t PR7         : 1;
            uint32_t PR8         : 1;
            uint32_t PR9         : 1;
            uint32_t PR10        : 1;
            uint32_t PR11        : 1;
            uint32_t PR12        : 1;
            uint32_t PR13        : 1;
            uint32_t PR14        : 1;
            uint32_t PR15        : 1;
            uint32_t PR16        : 1;
            uint32_t PR17        : 1;
            uint32_t PR18        : 1;
            uint32_t PR19        : 1;
            uint32_t reversed    : 12;
        } BITS;
    } PR;
} EXTI_Typedef;

#define RCC_ADDR            (0x40021000)
#define GPIOA_ADDR          (0x40010800)
#define GPIOA               ((GPIO_Typedef*) GPIOA_ADDR)
#define RCC                 ((RCC_Typedef*) RCC_ADDR)
#define EXTI_ADDR           (0x40010400)
#define EXTI                ((EXTI_Typedef*) EXTI_ADDR)
#define AFIO_ADDR           (0x40010000)
#define AFIO                ((AFIO_Typedef*) AFIO_ADDR)

#endif /* __STM32F103_ */
