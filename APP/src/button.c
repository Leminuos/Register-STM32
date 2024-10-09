#include "stm32f103.h"
#include "button.h"

Debouncer_Typedef Debouncer;

void EXTIConfig(void)
{
    /* Cau hinh ngat EXTI1 */
    AFIO->EXTICR1.BITS.EXTI1 = 0x00;
    EXTI->IMR.BITS.MR1 = 0x01;
    EXTI->FTSR.BITS.TR1 = 0x01;
    
    /* Cau hinh ngat NVIC */
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_SetPriority(EXTI1_IRQn, 0X01);
    
    /* Bat ngat toan cuc */
    __ASM("CPSIE I");
}

void ButtonConfig(void)
{
    /* Cau hinh clock port A */
    RCC->APB2ENR.BITS.IOPAEN = 1;
    
    /* Cau hinh input GPIOA pin 1 */
    GPIOA->CRL.BITS.MODE1 = 0x00;
    GPIOA->CRL.BITS.CNF1 = 0X02;
    GPIOA->ODR.BITS.ODR1 = 1;

    uint8_t i = 0;

    Debouncer.index = 0;
    Debouncer.pullType = 1;
    Debouncer.pressTime = 0;
    Debouncer.releaseTime = 0;
    Debouncer.lastPressTime = 1;
    Debouncer.status = STATUS_BUTTON_IDLE;
    
    for (i = 0; i < BUTTON_FILTER_ORDER; ++i)
        Debouncer.filter[i] = 0;
}

void RegisterClickFunction(CallBackFunction __function)
{
    Debouncer.ButtonHook.ClickFunc = __function;
}

void RegisterDoubleClickFunction(CallBackFunction __function)
{
    Debouncer.ButtonHook.DoubleClickFunc = __function;
}

void RegisterLongPressFunction(CallBackFunction __function)
{
    Debouncer.ButtonHook.LongPressFunc = __function;
}

void ButtonProcess(void)
{
    uint8_t i = 0;
    uint8_t sum = 0;
    BUTTON_STATUS btnSts = STATUS_BUTTON_IDLE;

    Debouncer.filter[Debouncer.index] = (GPIOA->IDR.BITS.IDR1) ^ Debouncer.pullType;

    for (i = 0; i < BUTTON_FILTER_ORDER; ++i)
    {
        sum = sum + Debouncer.filter[i];
    }

    Debouncer.index++;

    if (Debouncer.index >= BUTTON_FILTER_ORDER)
    {
        Debouncer.index = 0;
    }

    btnSts = ((sum == BUTTON_FILTER_ORDER) ? STATUS_BUTTON_PRESS : STATUS_BUTTON_RELEASE);

    if (Debouncer.status != btnSts)
    {
        Debouncer.status = btnSts;
        if (Debouncer.status == STATUS_BUTTON_PRESS) Debouncer.pressTime = 0;
        else Debouncer.releaseTime = 0;
    }
    else
    {
        if (Debouncer.status == STATUS_BUTTON_PRESS && Debouncer.pressTime < 0xFFFFFFFFU)
        {
            Debouncer.pressTime++;
        }
        else if (Debouncer.status == STATUS_BUTTON_RELEASE && Debouncer.releaseTime < 0xFFU)
        {
            Debouncer.releaseTime++;
        }
    }
}

uint8_t ButtonPress(void)
{
    return (Debouncer.status == STATUS_BUTTON_PRESS) ? 0xFF : 0x00;
}

uint8_t ButtonRelease(void)
{
    return (Debouncer.status == STATUS_BUTTON_RELEASE) ? 0xFF : 0x00;
}

uint8_t nClicks = 0;
uint8_t lastReleaseTime = 0;

BUTTON_STATUS ButtonClick(void)
{
    if (ButtonPress() && Debouncer.pressTime - Debouncer.lastPressTime > BUTTON_LONG_TIME)
    {
        Debouncer.lastPressTime = Debouncer.pressTime;

        if (Debouncer.ButtonHook.LongPressFunc)
            Debouncer.ButtonHook.LongPressFunc();
        
        return STATUS_BUTTON_LONG_PRESS;
    }

    if (ButtonRelease())
    {
        if (Debouncer.pressTime > BUTTON_DEBOUNCE_TIME && Debouncer.pressTime < BUTTON_LONG_TIME)
        {
            nClicks++;
            Debouncer.pressTime = 0;
        }

        if (Debouncer.releaseTime > BUTTON_RELEASE_TIME)
        {
            if (nClicks == 1)
            {
                nClicks = 0;
                if (Debouncer.ButtonHook.ClickFunc)
                    Debouncer.ButtonHook.ClickFunc();

                return STATUS_BUTTON_CLICK;
            }
            else if (nClicks == 2)
            {
                nClicks = 0;
                if (Debouncer.ButtonHook.DoubleClickFunc)
                    Debouncer.ButtonHook.DoubleClickFunc();
                
                return STATUS_BUTTON_DOUBLE_CLICK;
            }

            nClicks = 0;
            Debouncer.lastPressTime = 0;
            Debouncer.pressTime = 0;
            return STATUS_BUTTON_IDLE;
        }
    }

    return STATUS_BUTTON_IDLE;
}
