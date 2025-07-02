#ifndef __PWR__
#define __PWR__

#include "stm32f103.h"
#include "stm32_hal_util.h"

typedef struct
{
    uint32_t PVDLevel;  /*!< PVDLevel: Specifies the PVD detection level.
                            This parameter can be a value of @ref PWR_PVD_detection_level */

    uint32_t Mode;      /*!< Mode: Specifies the operating mode for the selected pins.
                            This parameter can be a value of @ref PWR_PVD_Mode */
}PWR_PVDTypeDef;

/** @defgroup PWR_SLEEP_mode_entry PWR SLEEP mode entry
  * @{
  */
#define PWR_SLEEPENTRY_WFI              ((uint8_t)0x01)
#define PWR_SLEEPENTRY_WFE              ((uint8_t)0x02)

/** @defgroup PWR_STOP_mode_entry PWR STOP mode entry
  * @{
  */
#define PWR_STOPENTRY_WFI               ((uint8_t)0x01)
#define PWR_STOPENTRY_WFE               ((uint8_t)0x02)

/**
  * @brief Enable interrupt on PVD Exti Line 16.
  * @retval None.
  */
#define __PWR_PVD_EXTI_ENABLE_IT()     (EXTI->IMR.BITS.MR16 = ENABLE)

/**
  * @brief Disable interrupt on PVD Exti Line 16. 
  * @retval None.
  */
#define __PWR_PVD_EXTI_DISABLE_IT()     (EXTI->IMR.BITS.MR16 = DISABLE)

/**
  * @brief Enable event on PVD Exti Line 16.
  * @retval None.
  */
#define __PWR_PVD_EXTI_ENABLE_EVENT()   (EXTI->EMR.BITS.MR16 = ENABLE)

/**
  * @brief Disable event on PVD Exti Line 16.
  * @retval None.
  */
#define __PWR_PVD_EXTI_DISABLE_EVENT()  (EXTI->EMR.BITS.MR16 = DISABLE)

/**
  * @brief  PVD EXTI line configuration: set falling edge trigger.  
  * @retval None.
  */
#define __PWR_PVD_EXTI_ENABLE_FALLING_EDGE()  (EXTI->FTSR.BITS.TR16 = ENABLE)

/**
  * @brief Disable the PVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __PWR_PVD_EXTI_DISABLE_FALLING_EDGE()  (EXTI->FTSR.BITS.TR16 = DISABLE)

/**
  * @brief  PVD EXTI line configuration: set rising edge trigger.
  * @retval None.
  */
#define __PWR_PVD_EXTI_ENABLE_RISING_EDGE()   (EXTI->RTSR.BITS.TR16 = ENABLE)

/**
  * @brief Disable the PVD Extended Interrupt Rising Trigger.
  * This parameter can be:
  * @retval None.
  */
#define __PWR_PVD_EXTI_DISABLE_RISING_EDGE()  (EXTI->RTSR.BITS.TR16 = DISABLE)

/**
  * @brief Check whether the specified PVD EXTI interrupt flag is set or not.
  * @retval EXTI PVD Line Status.
  */
#define __PWR_PVD_EXTI_GET_FLAG()       (EXTI->PR.BITS.PR16)

/**
  * @brief Clear the PVD EXTI flag.
  * @retval None.
  */
#define __PWR_PVD_EXTI_CLEAR_FLAG()     (EXTI->PR.BITS.PR16 = DISABLE)


/* Low Power modes configuration functions ************************************/
void PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry);
void PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);
void PWR_EnterSTANDBYMode(void);

void PWR_EnableSleepOnExit(void);
void PWR_DisableSleepOnExit(void);
void PWR_EnableSEVOnPend(void);
void PWR_DisableSEVOnPend(void);

void PWR_PVD_IRQHandler(void);
void PWR_PVDCallback(void);

#endif /* __PWR__ */

