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
            uint32_t            : 1;
            uint32_t HSITRIM    : 5;    /* Internal high-speed clock trimming */
            uint32_t HSICAL     : 8;    /* Internal high-speed clock calibration */
            uint32_t HSEON      : 1;    /* HSE clock enable */
            uint32_t HSERDY     : 1;    /* External high-speed clock ready flag */
            uint32_t HSEBYP     : 1;    /* External high-speed clock bypass */
            uint32_t CSSON      : 1;    /* Clock security system enable */
            uint32_t            : 4;
            uint32_t PLLON      : 1;    /* PLL enable */
            uint32_t PLLRDY     : 1;    /* PLL clock ready flag */
            uint32_t            : 6;
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
            uint32_t            : 1;
            uint32_t MCO        : 3;    /* Microcontroller clock output */
            uint32_t            : 5;
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
            uint32_t            : 2;
            uint32_t CSSF       : 1;    /* Clock security system interrupt flag */
            uint32_t LSIRDYIE   : 1;    /* LSI ready interrupt enable */
            uint32_t LSERDYIE   : 1;    /* LSE ready interrupt enable */
            uint32_t HSIRDYIE   : 1;    /* HSI ready interrupt enable */
            uint32_t HSERDYIE   : 1;    /* HSE ready interrupt enable */
            uint32_t PLLRDYIE   : 1;    /* PLL ready interrupt enable */
            uint32_t            : 2;
            uint32_t LSIRDYC    : 1;    /* LSI ready interrupt clear */
            uint32_t LSERDYC    : 1;    /* LSE ready interrupt clear */
            uint32_t HSIRDYC    : 1;    /* HSI ready interrupt clear */
            uint32_t HSERDYC    : 1;    /* HSE ready interrupt clear */
            uint32_t PLLRDYC    : 1;    /* PLL ready interrupt clear */
            uint32_t CSSC       : 1;    /* Clock security system interrupt clear */
            uint32_t            : 8;
        } BITS;
    } CIR;                              /* Clock interrupt register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t AFIORST    : 1;    /* Alternate function IO reset */
            uint32_t            : 1;
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
            uint32_t            : 3;
            uint32_t TIM9RST    : 1;    /* TIM8 timer reset */
            uint32_t TIM10RST   : 1;    /* TIM10 timer reset */
            uint32_t TIM11RST   : 1;    /* TIM11 timer reset */
            uint32_t            : 10;
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
            uint32_t            : 2;
            uint32_t WWDGRST    : 1;    /* Window watchdog reset */
            uint32_t SPI2RST    : 1;    /* SPI2 reset */
            uint32_t SPI3RST    : 1;    /* SPI3 reset */
            uint32_t            : 1;
            uint32_t USART2RST  : 1;    /* USART2 reset */
            uint32_t USART3RST  : 1;    /* USART3 reset */
            uint32_t USART4RST  : 1;    /* USART4 reset */
            uint32_t USART5RST  : 1;    /* USART5 reset */
            uint32_t I2C1RST    : 1;    /* I2C1 reset */
            uint32_t I2C2RST    : 1;    /* I2C2 reset */
            uint32_t USBRST     : 1;    /* USBRST reset */
            uint32_t            : 1;
            uint32_t CANRST     : 1;    /* CANRST reset */
            uint32_t            : 1;
            uint32_t BKPRST     : 1;    /* Backup interface reset */
            uint32_t PWRRST     : 1;    /* Power interface reset */
            uint32_t DACRST     : 1;    /* DAC interface reset */
            uint32_t            : 1;
        } BITS;
    } APB1RSTR;                         /* APB1 peripheral reset register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000014 */

        struct {
            uint32_t DMA1EN     : 1;    /* DMA1 clock enable */
            uint32_t DMA2EN     : 1;    /* DMA2 clock enable */
            uint32_t SRAMEN     : 1;    /* SRAM interface clock enable */
            uint32_t            : 1;
            uint32_t FLITFEN    : 1;    /* FLITF clock enable */
            uint32_t            : 1;
            uint32_t CRCEN      : 1;    /* CRC clock enable */
            uint32_t            : 1;
            uint32_t FSMCEN     : 1;    /* FSMC clock enable */
            uint32_t            : 1;
            uint32_t SDIOEN     : 1;    /* SDIO clock enable */
            uint32_t            : 21;
        } BITS;
    } AHBENR;                           /* AHB peripheral clock enable register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t AFIOEN     : 1;    /* Alternate function IO clock enable */
            uint32_t            : 1;
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
            uint32_t            : 3;
            uint32_t TIM9EN     : 1;    /* TIM9 Timer clock enable */
            uint32_t TIM10EN    : 1;    /* TIM10 Timer clock enable */
            uint32_t TIM11EN    : 1;    /* TIM11 Timer clock enable */
            uint32_t            : 10;
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
            uint32_t            : 2;
            uint32_t WWDGEN     : 1;    /* Window watchdog clock enable */
            uint32_t            : 2;
            uint32_t SPI2EN     : 1;    /* SPI2 clock enable */
            uint32_t SPI3EN     : 1;    /* SPI3 clock enable */
            uint32_t            : 1;
            uint32_t USART2EN   : 1;    /* USART2 clock enable */
            uint32_t USART3EN   : 1;    /* USART3 clock enable */
            uint32_t USART4EN   : 1;    /* USART4 clock enable */
            uint32_t USART5EN   : 1;    /* USART5 clock enable */
            uint32_t I2C1EN     : 1;    /* I2C1 clock enable */
            uint32_t I2C2EN     : 1;    /* I2C2 clock enable */
            uint32_t USBEN      : 1;    /* USBEN clock enable */
            uint32_t            : 1;
            uint32_t CANEN      : 1;    /* CANEN clock enable */
            uint32_t            : 1;
            uint32_t BKPEN      : 1;    /* Backup interface clock enable */
            uint32_t PWREN      : 1;    /* Power interface clock enable */
            uint32_t DACEN      : 1;    /* DAC interface clock enable */
            uint32_t            : 2;
        } BITS;
    } APB1ENR;                          /* APB1 peripheral clock enable register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x00000000 */

        struct {
            uint32_t LSEON      : 1;    /* External low-speed oscillator enable */
            uint32_t LSERDY     : 1;    /* External low-speed oscillator ready */
            uint32_t LSEBYP     : 1;    /* External low-speed oscillator bypass */
            uint32_t            : 5;
            uint32_t RTCSEL     : 2;    /* RTC clock source selection */
            uint32_t            : 5;
            uint32_t RTCEN      : 1;    /* RTC clock enable */
            uint32_t BDRST      : 1;    /* Backup domain software reset */
            uint32_t            : 15;
        } BITS;
    } BDCR;                             /* Backup domain control register */

    __IO union {
        uint32_t REG;                   /* Reset value: : 0x0C000000 */

        struct {
            uint32_t LSION      : 1;    /* Internal low-speed oscillator enable */
            uint32_t LSIRDY     : 1;    /* Ixternal low-speed oscillator ready */
            uint32_t            : 22;
            uint32_t RMVF       : 1;    /* Remove reset flag */
            uint32_t            : 1;
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
            uint32_t            : 16;
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
            uint32_t            : 16;
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
            uint32_t            : 16;
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
            uint32_t             : 15;
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
            uint32_t             : 24;
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
            uint32_t                    : 3;
            uint32_t SWJ_CFG            : 3;
            uint32_t                    : 5;
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
            uint32_t             : 16;
        } BITS;     /* External interrupt configuration register 1 */
    } EXTICR1;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI4       : 4;       /* EXTI x configuration (x = 0 to 3)*/
            uint32_t EXTI5       : 4;
            uint32_t EXTI6       : 4;
            uint32_t EXTI7       : 4;
            uint32_t             : 16;
        } BITS;     /* External interrupt configuration register 2 */
    } EXTICR2;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI8       : 4;       /* EXTI x configuration (x = 0 to 3)*/
            uint32_t EXTI9       : 4;
            uint32_t EXTI10      : 4;
            uint32_t EXTI11      : 4;
            uint32_t             : 16;
        } BITS;     /* External interrupt configuration register 3 */
    } EXTICR3;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EXTI12      : 4;       /* EXTI x configuration (x = 0 to 3)*/
            uint32_t EXTI13      : 4;
            uint32_t EXTI14      : 4;
            uint32_t EXTI15      : 4;
            uint32_t             : 16;
        } BITS;     /* External interrupt configuration register 3 */
    } EXTICR4;
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t             : 5;
            uint32_t TIM9_REMAP  : 1;
            uint32_t TIM10_REMAP : 1;
            uint32_t TIM11_REMAP : 1;
            uint32_t TIM13_REMAP : 1;
            uint32_t TIM14_REMAP : 1;
            uint32_t FSMC_NADV   : 1;
            uint32_t             : 21;
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
            uint32_t             : 12;
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
            uint32_t             : 12;
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
            uint32_t             : 12;
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
            uint32_t             : 12;
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
            uint32_t                : 12;
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
            uint32_t             : 12;
        } BITS;
    } PR;
} EXTI_Typedef;

typedef struct {
    union {
        uint32_t REG;

        struct {
            uint32_t CEN         : 1;   /* Counter Enable */
                                        /* Note: External clock, gated mode and encoder mode can work only              */
                                        /* if the CEN bit has been previously set by software.                          */
                                        /* However trigger mode can set the CEN bit automatically by hardware.          */
                                        /* CEN is cleared automatically in one-pulse mode, when an update event occurs. */

            uint32_t UDIS        : 1;   /* Update Event disable */
                                        /* This bit is set and cleared by software to enable/disable UEV event generation. */

            uint32_t URS         : 1;   /* Update request source */
                                        /* This bit is set and cleared by software to select the UEV event sources. */
            
            uint32_t OPM         : 1;   /* One-pulse mode */
                                        /* 0: Counter is not stopped at update event */
                                        /* 1: Counter stops counting at the next update event (clearing the bit CEN). */
            
            uint32_t DIR         : 1;   /* Direction */
                                        /* 0: Counter used as upcounter */
                                        /* 1: Counter used as downcounter */
                                        /* This bit is read only when the timer is configured in Center-aligned mode or Encoder mode. */
            
            uint32_t CMS         : 2;   /* Center-aligned mode selection */
                                        /* 00: Edge-aligned mode. The counter counts up or down depending on the direction bit (DIR). */
                                        /* 01: Center-aligned mode 1. The counter counts up and down alternativel.
                                           Output compare interrupt flags are set only when the counter is counting down. */
                                        /* 10: Center-aligned mode 2. The counter counts up and down alternatively.
                                           Output compare interrupt flags are set only when the counter is counting up. */
                                        /* 11: Center-aligned mode 3. The counter counts up and down alternatively.
                                           Output compare interrupt flags are set only when the counter is counting up or down. */
            
            uint32_t ARPE        : 1;   /* Auto-reload preload enable */
                                        /* 0: TIMx_ARR register is not buffered */
                                        /* 1: TIMx_ARR register is buffered */
            
            uint32_t CKD         : 1;   /* Clock division */
                                        /* 00: tDTS = tCK_INT */
                                        /* 01: tDTS = 2 × tCK_INT */
                                        /* 10: tDTS = 4 × tCK_INT */
                                        /* 11: Reserved */
            
            uint32_t             : 22;
        } BITS;
    } CR1;

    union {
        uint32_t REG;

        struct {
            uint32_t             : 3;

            uint32_t CCDS        : 1;   /* Capture/compare DMA selection */
                                        /* 0: CCx DMA request sent when CCx event occurs */
                                        /* 1: CCx DMA requests sent when update event occurs */

            uint32_t MMS         : 3;   /* Master mode selection */
                                        /* These bits allow to select the information to be sent in master mode to slave timers for synchronization (TRGO). */

            uint32_t TI1S        : 1;   /* TI1 selection */
                                        /* 0: The TIMx_CH1 pin is connected to TI1 input */
                                        /* 1: The TIMx_CH1, CH2 and CH3 pins are connected to the TI1 input (XOR combination) */

            uint32_t             : 24;
        } BITS;
    } CR2;

    union {
        uint32_t REG;

        struct {
            uint32_t SMS         : 3;   /* Slave mode selection */
            uint32_t             : 1;
            uint32_t TS          : 3;   /* Trigger selection */
            uint32_t MSM         : 1;   /* Master/Slave mode */
            uint32_t ETF         : 4;   /* External trigger filter */
            uint32_t ETPS        : 2;   /* External trigger prescaler */
            uint32_t ECE         : 1;   /* External clock enable */
            uint32_t ETP         : 1;   /* External trigger polarity */
            uint32_t             : 16;

        } BITS;
    } SMCR;                             /* TIMx slave mode control register */

    union {
        uint32_t REG;

        struct {
            uint32_t UIE         : 1;   /* Update interrupt enable */
            uint32_t CC1IE       : 1;   /* Capture/Compare 1 interrupt enable */
            uint32_t CC2IE       : 1;   /* Capture/Compare 2 interrupt enable */
            uint32_t CC3IE       : 1;   /* Capture/Compare 3 interrupt enable */
            uint32_t CC4IE       : 1;   /* Capture/Compare 4 interrupt enable */
            uint32_t             : 1;
            uint32_t TIE         : 1;   /* Trigger interrupt enable */
            uint32_t             : 1;
            uint32_t UDE         : 1;   /* Update DMA request enable */
            uint32_t CC1DE       : 1;   /* Capture/Compare 1 DMA request enable */
            uint32_t CC2DE       : 1;   /* Capture/Compare 2 DMA request enable */
            uint32_t CC3DE       : 1;   /* Capture/Compare 3 DMA request enable */
            uint32_t CC4DE       : 1;   /* Capture/Compare 4 DMA request enable */
            uint32_t             : 1;
            uint32_t TDE         : 1;   /* Trigger DMA request enable */
            uint32_t             : 17;
        } BITS;
    } DIER;                             /* TIMx DMA/Interrupt enable register */

    union {
        uint32_t REG;

        struct {
            uint32_t UIF         : 1;   /* Update interrupt flag */
            uint32_t CC1IF       : 1;   /* Capture/Compare 1 interrupt flag */
            uint32_t CC2IF       : 1;   /* Capture/Compare 2 interrupt flag */
            uint32_t CC3IF       : 1;   /* Capture/Compare 3 interrupt flag */
            uint32_t CC4IF       : 1;   /* Capture/Compare 4 interrupt flag */
            uint32_t             : 1;
            uint32_t TIF         : 1;   /*  Trigger interrupt flag */
            uint32_t             : 2;
            uint32_t CC10F       : 1;   /* Capture/Compare 1 overcapture flag */
            uint32_t CC20F       : 1;   /* Capture/Compare 2 overcapture flag */
            uint32_t CC30F       : 1;   /* Capture/Compare 3 overcapture flag */
            uint32_t CC40F       : 1;   /* Capture/Compare 4 overcapture flag */
            uint32_t             : 19;
        } BITS;
    } SR;                               /* TIMx status register */

    union {
        uint32_t REG;

        struct {
            uint32_t UG          : 1;   /* Update generation */
            uint32_t CC1G        : 1;   /* Capture/compare 1 generation */
            uint32_t CC2G        : 1;   /* Capture/compare 2 generation */
            uint32_t CC3G        : 1;   /* Capture/compare 3 generation */
            uint32_t CC4G        : 1;   /* Capture/compare 4 generation */
            uint32_t             : 1;
            uint32_t TG          : 1;   /* Trigger generation*/
            uint32_t             : 25;
        } BITS;
    } EGR;                              /* TIMx event generation register */

    union {
        uint32_t REG;

        struct {
            uint32_t CC1S        : 2;   /* Capture/Compare 1 selection */
            uint32_t OC1FE       : 1;   /* Output compare 1 fast enable */
            uint32_t OC1PE       : 1;   /* Output compare 1 preload enable */
            uint32_t OC1M        : 3;   /* Output compare 1 mode */
            uint32_t OC1CE       : 1;   /* Output compare 1 clear enable */
            uint32_t CC2S        : 2;   /* Capture/Compare 2 selection */
            uint32_t OC2FE       : 1;   /* Output compare 2 fast enable */
            uint32_t OC2PE       : 1;   /* Output compare 2 preload enable */
            uint32_t OC2M        : 3;   /* Output compare 2 mode */
            uint32_t OC2CE       : 1;   /* Output compare 2 clear enable */
        } BITS_O;                       /* Output compare mode */

        struct {
            uint32_t CC1S        : 2;   /* Capture/Compare 1 selection */
            uint32_t IC1PSC      : 2;   /* Input capture 1 prescaler */
            uint32_t IC1F        : 4;   /* Input capture 1 filter */
            uint32_t CC2S        : 2;   /* Capture/Compare 2 selection */
            uint32_t IC2PSC      : 2;   /* Input capture 2 prescaler */
            uint32_t IC2F        : 4;   /* Input capture 2 filter*/
        } BITS_I;                       /* Input compare mode */

    } CCMR1;                            /* TIMx capture/compare mode register 1 */

    union {
        uint32_t REG;

        struct {
            uint32_t CC3S        : 2;   /* Capture/Compare 3 selection */
            uint32_t OC3FE       : 1;   /* Output compare 3 fast enable */
            uint32_t OC3PE       : 1;   /* Output compare 3 preload enable */
            uint32_t OC3M        : 3;   /* Output compare 3 mode */
            uint32_t OC3CE       : 1;   /* Output compare 3 clear enable */
            uint32_t CC4S        : 2;   /* Capture/Compare 4 selection */
            uint32_t OC4FE       : 1;   /* Output compare 4 fast enable */
            uint32_t OC4PE       : 1;   /* Output compare 4 preload enable */
            uint32_t OC4M        : 3;   /* Output compare 4 mode */
            uint32_t OC4CE       : 1;   /* Output compare 4 clear enable */
        } BITS_O;                       /* Output compare mode */

        struct {
            uint32_t CC3S        : 2;   /* Capture/Compare 3 selection */
            uint32_t IC3PSC      : 2;   /* Input capture 3 prescaler */
            uint32_t IC3F        : 4;   /* Input capture 3 filter */
            uint32_t CC4S        : 2;   /* Capture/Compare 4 selection */
            uint32_t IC4PSC      : 2;   /* Input capture 4 prescaler */
            uint32_t IC4F        : 4;   /* Input capture 4 filter*/
        } BITS_I;                       /* Input compare mode */

    } CCMR2;                            /* TIMx capture/compare mode register 2 */

    union {
        uint32_t REG;

        struct {
            uint32_t CC1E        : 1;   /* Capture/Compare 1 output enable */
            uint32_t CC1P        : 1;   /* Capture/Compare 1 output polarity */
            uint32_t             : 2;
            uint32_t CC2E        : 1;   /* Capture/Compare 2 output enable */
            uint32_t CC2P        : 1;   /* Capture/Compare 2 output polarity */
            uint32_t             : 2;
            uint32_t CC3E        : 1;   /* Capture/Compare 3 output enable */
            uint32_t CC3P        : 1;   /* Capture/Compare 3 output polarity */
            uint32_t             : 2;
            uint32_t CC4E        : 1;   /* Capture/Compare 4 output enable */
            uint32_t CC4P        : 1;   /* Capture/Compare 4 output polarity */
            uint32_t             : 18;
        } BITS;
    } CCER;                             /* TIMx capture/compare enable register */

    union {
        uint32_t REG;

        struct {
            uint32_t CNT         : 16;
            uint32_t             : 16;
        } BITS;
    } CNT;                              /* TIMx counter */

    union {
        uint32_t REG;

        struct {
            uint32_t PSC         : 16;  /* The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1). */
            uint32_t             : 16;
        } BITS;
    } PSC;                              /* TIMx prescaler */

    union {
        uint32_t REG;

        struct {
            uint32_t ARR         : 16;  /* ARR is the value to be loaded in the actual auto-reload register. */
            uint32_t             : 16;
        } BITS;
    } ARR;                              /* TIMx auto-reload register */

    uint32_t reserved0;

    union {
        uint32_t REG;

        struct {
            uint32_t CCR1        : 16;  /* Capture/Compare 1 value. */
            uint32_t             : 16;
        } BITS;
    } CCR1;                             /* TIMx capture/compare register 1 */

    union {
        uint32_t REG;

        struct {
            uint32_t CCR2        : 16;  /* Capture/Compare 2 value. */
            uint32_t             : 16;
        } BITS;
    } CCR2;                             /* TIMx capture/compare register 2 */

    union {
        uint32_t REG;

        struct {
            uint32_t CCR3        : 16;  /* Capture/Compare 3 value. */
            uint32_t             : 16;
        } BITS;
    } CCR3;                             /* TIMx capture/compare register 3 */

    union {
        uint32_t REG;

        struct {
            uint32_t CCR4        : 16;  /* Capture/Compare 4 value. */
            uint32_t             : 16;
        } BITS;
    } CCR4;                             /* TIMx capture/compare register 4 */

    uint32_t                     : 32;

    union {
        uint32_t REG;

        struct {
            uint32_t DBA         : 5;   /* DMA base address. */
            uint32_t             : 3;
            uint32_t DBL         : 5;   /* DMA burst length. */
            uint32_t             : 19;
        } BITS;
    } DCR;                              /* TIMx DMA control register */

    union {
        uint32_t REG;

        struct {
            uint32_t DMAB        : 16;  /* DMA register for burst accesses. */
            uint32_t             : 16;
        } BITS;
    } DMAR;                             /* TIMx DMA address for full transfer */
} TIMx_Typedef;

typedef struct {
    union {
        uint32_t REG;
        
        struct {
            uint32_t PE          : 1;   /* Parity error */
            uint32_t FE          : 1;   /* Framing error */
            uint32_t NE          : 1;   /* Noise error flag */
            uint32_t ORE         : 1;   /* Overrun error */
            uint32_t IDLE        : 1;   /* IDLE line detected */
            uint32_t RXNE        : 1;   /* Read data register not empty */
            uint32_t TC          : 1;   /* Transmission complete */
            uint32_t TXE         : 1;   /* Transmit data register empty */
            uint32_t LBD         : 1;   /* LIN break detection flag */
            uint32_t CTS         : 1;   /* CTS flag */
            uint32_t             : 22;
        } BITS;
    } SR;                               /* Status register */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t DR          : 8;   /* Data value */
            uint32_t             : 23;
        } BITS;
    } DR;                               /* Data register */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t Fraction    : 4;   /* fraction of USARTDIV */
            uint32_t Mantissa    : 12;  /* mantissa of USARTDIV */
            uint32_t             : 16;
        } BITS;               
    } BRR;                              /* Baud rate register */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t SBK         : 1;   /* Send break */
            uint32_t RWU         : 1;   /* Receiver wakeup */
            uint32_t RE          : 1;   /* Receiver enable */
            uint32_t TE          : 1;   /* Transmitter enable */
            uint32_t IDLEIE      : 1;   /* IDLE interrupt enable */
            uint32_t RXNEIE      : 1;   /* RXNE interrupt enable */
            uint32_t TCIE        : 1;   /* Transmission complete interrupt enable */
            uint32_t TXEIE       : 1;   /* TXE interupt enable */
            uint32_t PEIE        : 1;   /* PE interrupt enable */
            uint32_t PS          : 1;   /* Parity selection */
            uint32_t PCE         : 1;   /* Parity control enable */
            uint32_t WAKE        : 1;   /* Wakeup method */
            uint32_t M           : 1;   /* Word length */
            uint32_t UE          : 1;   /* USART enable */
            uint32_t             : 18;
        } BITS;
    } CR1;                              /* Control register 1 */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t ADD         : 4;   /* Address of the USART node */
            uint32_t             : 1;
            uint32_t LBDL        : 1;   /* LIN break detection length */
            uint32_t LBDIE       : 1;   /* LIN break detection interrupt enable */
            uint32_t             : 1;
            uint32_t LBCL        : 1;   /* Last bit clock pulse */
            uint32_t CPHA        : 1;   /* Clock phase */
            uint32_t CPOL        : 1;   /* Clock polaritye */
            uint32_t CLKEN       : 1;   /* Clock enable */
            uint32_t STOP        : 2;   /* STOP bit */
            uint32_t LINEN       : 1;   /* LIN mode enable */
            uint32_t             : 17;
        } BITS;
    } CR2;                              /* Control register 2 */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t EIE         : 1;   /* Error interrupt enable */
            uint32_t IREN        : 1;   /* IrDA mode enable */
            uint32_t IRLP        : 1;   /* IrDA low power */
            uint32_t HDSEL       : 1;   /* Half duplex selection */
            uint32_t NACK        : 1;   /* Smartcard NACK enable */
            uint32_t SCEN        : 1;   /* Smartcard mode enable */
            uint32_t DMAR        : 1;   /* DMA enable receiver */
            uint32_t DMAT        : 1;   /* DMA enable transmitter */
            uint32_t RTSE        : 1;   /* RTS enable */
            uint32_t CTSE        : 1;   /* CTS enable */
            uint32_t CTSIE       : 1;   /* CTS interrupt enable */
            uint32_t             : 21;
        } BITS;
    } CR3;                              /* Control register 3 */
    
    union {
        uint32_t REG;
        
        struct {
            uint32_t PSC         : 8;   /* Prescaler value */
            uint32_t GT          : 8;   /* Guard time value */
            uint32_t             : 16;
        } BITS;
    } GTPR;                             /* Guard time and prescaler register */
} USART_Typedef;

typedef struct {
    __IO union {
        uint32_t REG;
        
        struct {
            __IO uint32_t PE          : 1;   /* Peripheral enable */
            __IO uint32_t SMBUS       : 1;   /* SMBus mode */
                 uint32_t             : 1;
            __IO uint32_t SMBTYPE     : 1;   /* SMBus type */
            __IO uint32_t ENARP       : 1;   /* ARP enable */   
            __IO uint32_t ENPEC       : 1;   /* PEC enable */
            __IO uint32_t ENGC        : 1;   /* General call enable */
            __IO uint32_t NOSTRETCH   : 1;   /* Clock stretching disable (Slave mode) */
            __IO uint32_t START       : 1;   /* Start generation */
            __IO uint32_t STOP        : 1;   /* Stop generation */
            __IO uint32_t ACK         : 1;   /* Acknowledge enable */
            __IO uint32_t POS         : 1;   /* Acknowledge/PEC Position (for data reception) */
            __IO uint32_t PEC         : 1;   /* Packet error checking */
            __IO uint32_t ALERT       : 1;   /* SMBus alert */
                 uint32_t             : 1;
            __IO uint32_t SWRST       : 1;   /* Software reset */
        } BITS;
    } CR1;                              /* Control Register 1 */
    
    __IO union {
        uint32_t REG;
        
        struct {
            __IO uint32_t FREQ        : 6;   /* Peripheral clock frequency */
                 uint32_t             : 2;
            __IO uint32_t ITERREN     : 1;   /* Error interrupt enable */
            __IO uint32_t ITEVTEN     : 1;   /* Event interrupt enable */
            __IO uint32_t ITBUFEN     : 1;   /* Buffer interrupt enable */
            __IO uint32_t DMAEN       : 1;   /* DMA requests enable */
            __IO uint32_t LAST        : 1;   /* DMA last transfer */
        } BITS;
    } CR2;                              /* Control Register 2 */
    
    __IO union {
        uint32_t REG;
        
        struct {
            __IO uint32_t ADD0            : 1;   /* Interface address */
            __IO uint32_t ADD7            : 7;   /* Interface address */
            __IO uint32_t ADD10           : 2;   /* Interface address */
                 uint32_t                 : 5;
            __IO uint32_t ADDMODE         : 1;   /* Addressing mode (slave mode) */
        } BITS;
    } OAR1;                             /* Own address register 1 */
    
    __IO union {
        uint32_t REG;
        
        struct {
            __IO uint32_t ENDUAL          : 1;   /* Dual addressing mode enable */
            __IO uint32_t ADD2            : 7;   /* Interface address */
        } BITS;
    } OAR2;                             /* Own address register 1 */
    
    __IO uint32_t DR;                   /* Data register */

    __IO union {
        uint32_t REG;
        
        struct {
            __I  uint32_t SB              : 1;   /* : Start bit (Master mode) */
            __I  uint32_t ADDR            : 1;   /* Address sent (master mode)/matched (slave mode) */
            __I  uint32_t BTF             : 1;   /* Byte transfer finished */
            __I  uint32_t ADD10           : 1;   /* 10-bit header sent (Master mode) */
            __I  uint32_t STOPF           : 1;   /* Stop detection (slave mode) */
                 uint32_t                 : 1;
            __I  uint32_t RxNE            : 1;   /* Data register not empty (receivers) */
            __I  uint32_t TxE             : 1;   /* Data register empty (transmitters) */
            __IO uint32_t BERR            : 1;   /* Bus error */
            __IO uint32_t ARLO            : 1;   /* Arbitration lost (master mode) */
            __IO uint32_t AF              : 1;   /* Acknowledge failure */
            __IO uint32_t OVR             : 1;   /* Overrun/Underrun */
            __IO uint32_t PECERR          : 1;   /* PEC Error in reception */
                 uint32_t                 : 1;
            __IO uint32_t TIMEOUT         : 1;   /* Timeout or Tlow error */
            __IO uint32_t SMBALERT        : 1;   /* SMBus alert */
        } BITS;
    } SR1;                              /* Status register 1 */
    
    __IO union {
        uint32_t REG;
        
        struct {
            __IO uint32_t MSL             : 1;   /* Master/slave */
            __IO uint32_t BUSY            : 1;   /* Bus busy */
            __IO uint32_t TRA             : 1;   /* Transmitter/receiver */
                 uint32_t                 : 1;
            __IO uint32_t GENCALL         : 1;   /* General call address (Slave mode) */
            __IO uint32_t SMBDEFAULT      : 1;   /* SMBus device default address (Slave mode) */
            __IO uint32_t SMBHOST         : 1;   /* SMBus host header (Slave mode) */
            __IO uint32_t DUALF           : 1;   /* Dual flag (Slave mode) */
            __IO uint32_t PEC             : 8;   /* Packet error checking register */
        } BITS;
    } SR2;                              /* Status register 2 */
    
    __IO union {
        uint32_t REG;
        
        struct {
            __IO uint32_t CCR             : 12;  /* Clock control register in Fm/Sm mode (Master mode) */
                 uint32_t                 : 2;
            __IO uint32_t DUTY            : 1;   /* Fm mode duty cycle */
            __IO uint32_t FS              : 1;   /* I2C master mode selection */
        } BITS;
    } CCR;                              /* Clock control register */
    
    __IO uint32_t TRISE;                /* Maximum rise time in Fm/Sm mode (Master mode) */
} I2C_Typedef;

#define RCC_ADDR            (0x40021000)
#define GPIOA_ADDR          (0x40010800)
#define GPIOB_ADDR          (0x40010C00)
#define GPIOA               ((GPIO_Typedef*) GPIOA_ADDR)
#define GPIOB               ((GPIO_Typedef*) GPIOB_ADDR)
#define RCC                 ((RCC_Typedef*)  RCC_ADDR)
#define EXTI_ADDR           (0x40010400)
#define EXTI                ((EXTI_Typedef*) EXTI_ADDR)
#define AFIO_ADDR           (0x40010000)
#define AFIO                ((AFIO_Typedef*) AFIO_ADDR)
#define TIM2_ADDR           (0x40000000)
#define TIM2                ((TIMx_Typedef*) TIM2_ADDR)
#define TIM3_ADDR           (0x40000400)
#define TIM3                ((TIMx_Typedef*) TIM3_ADDR)
#define USART1_ADDR         (0x40013800)
#define USART1              ((USART_Typedef*) USART1_ADDR)
#define USART2_ADDR         (0x40004400)
#define USART2              ((USART_Typedef*) USART2_ADDR)
#define USART3_ADDR         (0x40004800)
#define USART3              ((USART_Typedef*) USART3_ADDR)
#define UART4_ADDR          (0x40004C00)
#define UART4               ((USART_Typedef*) UART4_ADDR)
#define UART5_ADDR          (0x40005000)
#define UART5               ((USART_Typedef*) UART5_ADDR)
#define I2C1_ADDR           (0x40005400)
#define I2C1                ((I2C_Typedef*) I2C1_ADDR)

#endif /* __STM32F103_ */
