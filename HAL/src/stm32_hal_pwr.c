#include "stm32_hal_pwr.h"

/** @defgroup PWR_PVD_Mode_Mask PWR PVD Mode Mask
  * @{
  */ 
#define PVD_MODE_IT               0x00010000U
#define PVD_MODE_EVT              0x00020000U
#define PVD_RISING_EDGE           0x00000001U
#define PVD_FALLING_EDGE          0x00000002U


static void PWR_OverloadWfe(void)
{
  __asm volatile( "wfe" );
  __asm volatile( "nop" );
}

/** ============================================================================
                 ##### Peripheral Control functions #####
    ============================================================================

    *** Cấu hình Programmable voltage detector ***

        (+) PVD được sử dụng để giám sát power supply VDD/VDDA bằng cách so sánh nó
            với một ngưỡng được chọn bởi các bit PLS[2:0] trong thanh ghi PWR_CR.
        
        (+) PVD được kích hoạt bằng cách set bit PVDE bit.

        (+) Cờ PVDO trong thanh ghi PWR_CSR cho biết VDD/VDDA có cao hay hơn ngưỡng
            PVD hay không. Event được kết nối với đường EXTI line 16 và có thể tạo
            ra ngắt nếu được kích hoạt.

        (+) PVD sẽ dừng hoạt động ở chế độ Standby.
    
    *** Cấu hình chân WakeUp ***

        (+) Chân WakeUp được sử dụng để wake system từ chế độ Standby mode. Chân này
            được cấu hình ở input mode với điện trở pull-down và active khi có sườn
            lên (rising edge).

        (+) Có một chân Wake Up tại chân PA0

    *** Cấu hình low power mode ***

    Thiết bị hỗ trợ 3 chế độ low power mode:

        (+) Sleep mode: Clock CPU tắt, nhưng tất cả các ngoại vi vẫn hoạt động.

        (+) Stop mode: tất cả clock bị dừng.

        (+) Standby mode: Điện áp 1.8V power off.

    *** Chế độ Sleep ***

        (+) Entry mode:

            (++) Clear bit SLEEPDEEP trong thanh ghi System Control Register.

            (++) Vào chế độ Sleep bằng lệnh assembly WFI hoặc lệnh assembly WFE.

        (+) Exit mode:

            (++) Với chế độ vào WFI, bất kỳ ngắt ngoại vi nào được assert bởi NVIC
                đều có thể wake CPU khỏi chế độ Sleep.

            (++) Với chế độ vào WFE, bất kỳ wakeup event nào cũng có thể wake CPU
                khỏi chế độ Sleep:

                (+++) Ngắt ngoại vi bất kỳ và set bit SEVONPEND trong thanh ghi System Control.

                (+++) Bất kỳ đường EXTI nào (external hoặc internal) được cấu hình ở chế độ Event mode.
    
    *** Stop mode ***

    Chế độ Stop mode dựa trên chế độ deepsleep của Cortex-M3 kết hợp với việc tắt clock ngoại vi.

    Trong chế độ Stop mode: Tất cả clock trong miền 1.8V đều dừng. PLL, bộ dao động HSI và HSE bị
    vô hiệu hóa. SRAM và nội dung thanh ghi được giữ nguyên. Tất cả chân I/O giữ nguyên trạng thái
    như khi ở chế độ Run mode.

    Ngoài ra, voltage regulator có thể được cấu hình ở chế độ low-power mode để giảm mức tiêu thụ
    bằng cách set bit LPDS trong thanh ghi PWR_CR

        (+) Entry mode:

            (++) Set bit SLEEPDEEP trong thanh ghi System Control Register.

            (++) Clear bit PDDS trong thanh ghi Power Control Register.

            (++) Select chế độ voltage regulator mode thông qua bit LPDS trong thanh ghi Power Control Register.

            (++) Clear bit SLEEPDEEP trong thanh ghi System Control Register.

            Để vào chế độ stop mode, tất cả các bit EXTI Line pending, peripheral interrupt pending và
            RTC Alarm flag phải reset.

        (+) Exit mode:

            (++) Với WFI: bất kỳ đường EXTI (internel hoặc external) nào được cấu hình ở
                chế độ interrupt và cấu hình NVIC.

            (++) Với WFE: bất kỳ EXTI nào được cấu hình ở chế độ Event mode.

            Khi exit chế đố stop mode, HSI RC oscillator được chọn là system clock.

        (+) Wakeup Latency: HSI RC wakeup time + regulator wakeup time từ chế độ Low-power mode
    
    *** Standby mode ***

    Chế độ Standby mode cho phép đạt mức tiêu thụ điện năng thấp nhất. Nó dựa trên chế độ deepsleep
    của Cortex-M3 với voltage regulator bị disable. Do đó, miền 1.8V sẽ mất điện. PLL, HSI và HSE
    cũng bị tắt. SRAM và nội dung thanh ghi sẽ bị mất, ngoại trừ các thanh ghi Backup và mạch Standby.

    (+) Enter mode:

        (++) Sử dụng hàm PWR_EnterSTANDBYMode() để vào chế độ Standby.

    (+) Exit mode:

        (++)WKUP pin rising edge, RTC alarm event rising edge, external Reset in NRSTpin, IWDG Reset

    *** Auto wake up từ chế độ low power ***

        (+) MCU có thể được đánh thức từ chế độ low power bởi sự kiện RTC alarm event mà không cần
        phụ thuộc vào external interrupt (Auto-wakeup mode).

        (+) RTC hỗ trợ chế độ auto wakeup mode từ chế độ Stop và Standby:

            (++) Để đánh thức từ chế độ Stop bằng sự kiện RTC alarm event.
  */

/**
  * @brief  Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param  sConfigPVD: pointer to an PWR_PVDTypeDef structure that contains the configuration
  *         information for the PVD.
  * @note   Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each
  *         detection level.
  * @retval None
  */
void PWR_ConfigPVD(PWR_PVDTypeDef *sConfigPVD)
{
    /* Set PLS[7:5] bits according to PVDLevel value */
    PWR->CR.BITS.PLS = sConfigPVD->PVDLevel;
    
    /* Clear any previous config. Keep it clear if no event or IT mode is selected */
    __PWR_PVD_EXTI_DISABLE_EVENT();
    __PWR_PVD_EXTI_DISABLE_IT();
    __PWR_PVD_EXTI_DISABLE_FALLING_EDGE(); 
    __PWR_PVD_EXTI_DISABLE_RISING_EDGE();

    /* Configure interrupt mode */
    if((sConfigPVD->Mode & PVD_MODE_IT) == PVD_MODE_IT)
    {
        __PWR_PVD_EXTI_ENABLE_IT();
    }
    
    /* Configure event mode */
    if((sConfigPVD->Mode & PVD_MODE_EVT) == PVD_MODE_EVT)
    {
        __PWR_PVD_EXTI_ENABLE_EVENT();
    }
    
    /* Configure the edge */
    if((sConfigPVD->Mode & PVD_RISING_EDGE) == PVD_RISING_EDGE)
    {
        __PWR_PVD_EXTI_ENABLE_RISING_EDGE();
    }
    
    if((sConfigPVD->Mode & PVD_FALLING_EDGE) == PVD_FALLING_EDGE)
    {
        __PWR_PVD_EXTI_ENABLE_FALLING_EDGE();
    }
}

/**
 * @brief Enters Sleep mode.
 * @note  In Sleep mode, all I/O pins keep the same state as in Run mode.
 * @param Regulator: Regulator state as no effect in SLEEP mode -  allows to support portability from legacy software
 * @param SLEEPEntry: Specifies if SLEEP mode is entered with WFI or WFE instruction.
 *           When WFI entry is used, tick interrupt have to be disabled if not desired as 
 *           the interrupt wake up source.
 *           This parameter can be one of the following values:
 *            @arg PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
 *            @arg PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
 * @retval None
 */
void PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)
{
    /* No check on Regulator because parameter not used in SLEEP mode */
    /* Prevent unused argument(s) compilation warning */
    UNUSED(Regulator);

    /* Clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* Select SLEEP mode entry -------------------------------------------------*/
    if(SLEEPEntry == PWR_SLEEPENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
  * @brief Enters Stop mode. 
  * @note  In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note  When exiting Stop mode by using an interrupt or a wakeup event,
  *        HSI RC oscillator is selected as system clock.
  * @note  When the voltage regulator operates in low power mode, an additional
  *         startup delay is incurred when waking up from Stop mode. 
  *         By keeping the internal regulator ON during Stop mode, the consumption
  *         is higher although the startup time is reduced.    
  * @param Regulator: Specifies the regulator state in Stop mode.
  *          This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_ON: Stop mode with regulator ON
  *            @arg PWR_LOWPOWERREGULATOR_ON: Stop mode with low power regulator ON
  * @param STOPEntry: Specifies if Stop mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_STOPENTRY_WFI: Enter Stop mode with WFI instruction
  *            @arg PWR_STOPENTRY_WFE: Enter Stop mode with WFE instruction   
  * @retval None
  */
void PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry)
{
    /* Clear PDDS bit in PWR register to specify entering in STOP mode when CPU enter in Deepsleep */ 
    PWR->CR.BITS.PDDS = DISABLE;

    /* Select the voltage regulator mode by setting LPDS bit in PWR register according to Regulator parameter value */
    PWR->CR.BITS.LPDS = Regulator;

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* Select Stop mode entry --------------------------------------------------*/
    if(STOPEntry == PWR_STOPENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        PWR_OverloadWfe(); /* WFE redefine locally */
        PWR_OverloadWfe(); /* WFE redefine locally */
    }

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief Enters Standby mode.
  * @note  In Standby mode, all I/O pins are high impedance except for:
  *          - Reset pad (still available) 
  *          - TAMPER pin if configured for tamper or calibration out.
  *          - WKUP pin (PA0) if enabled.
  * @retval None
  */
void PWR_EnterSTANDBYMode(void)
{
    /* Select Standby mode */
    PWR->CR.BITS.PDDS = 0x01;

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
    __force_stores();
#endif
    /* Request Wait For Interrupt */
    __WFI();
}

/**
  * @brief Indicates Sleep-On-Exit when returning from Handler mode to Thread mode. 
  * @note Set SLEEPONEXIT bit of SCR register. When this bit is set, the processor 
  *       re-enters SLEEP mode when an interruption handling is over.
  *       Setting this bit is useful when the processor is expected to run only on
  *       interruptions handling.         
  * @retval None
  */
void PWR_EnableSleepOnExit(void)
{
    /* Set SLEEPONEXIT bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}

/**
  * @brief Disables Sleep-On-Exit feature when returning from Handler mode to Thread mode. 
  * @note Clears SLEEPONEXIT bit of SCR register. When this bit is set, the processor 
  *       re-enters SLEEP mode when an interruption handling is over.          
  * @retval None
  */
void PWR_DisableSleepOnExit(void)
{
    /* Clear SLEEPONEXIT bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}

/**
  * @brief Enables CORTEX M3 SEVONPEND bit. 
  * @note Sets SEVONPEND bit of SCR register. When this bit is set, this causes 
  *       WFE to wake up when an interrupt moves from inactive to pended.
  * @retval None
  */
void PWR_EnableSEVOnPend(void)
{
    /* Set SEVONPEND bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}

/**
  * @brief Disables CORTEX M3 SEVONPEND bit. 
  * @note Clears SEVONPEND bit of SCR register. When this bit is set, this causes 
  *       WFE to wake up when an interrupt moves from inactive to pended.         
  * @retval None
  */
void PWR_DisableSEVOnPend(void)
{
    /* Clear SEVONPEND bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}

/**
  * @brief  This function handles the PWR PVD interrupt request.
  * @note   This API should be called under the PVD_IRQHandler().
  * @retval None
  */
void PWR_PVD_IRQHandler(void)
{
    /* Check PWR exti flag */
    if(__PWR_PVD_EXTI_GET_FLAG() != DISABLE)
    {
        /* PWR PVD interrupt user callback */
        PWR_PVDCallback();

        /* Clear PWR Exti pending bit */
        __PWR_PVD_EXTI_CLEAR_FLAG();
    }
}

/**
  * @brief  PWR PVD interrupt callback
  * @retval None
  */
__weak void PWR_PVDCallback(void)
{
    /* NOTE : This function Should not be modified, when the callback is needed,
              the PWR_PVDCallback could be implemented in the user file
    */ 
}

