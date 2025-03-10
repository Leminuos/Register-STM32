#include "stm32f103.h"
#include "button.h"

Debouncer_Typedef       Debouncer[MULTI_BUTTON];
static ButtonHook_Typedef      ButtonHook;

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
    uint8_t i, j;
    GPIO_InitTypeDef GPIO_InitStruct;

    for (i = 0; i < MULTI_BUTTON; ++i)
    {
        GPIO_InitStruct.GPIO_Pin = ButtonConfigs[i].Pin;
        GPIO_InitStruct.GPIO_Mode = ButtonConfigs[i].Mode;
        GPIO_Init(ButtonConfigs[i].Port, &GPIO_InitStruct);
        
        Debouncer[i].index = 0;
        Debouncer[i].nClicks = 0;
        Debouncer[i].pullType = 1;
        Debouncer[i].pressTime = 0;
        Debouncer[i].releaseTime = 0;
        Debouncer[i].lastPressTime = 1;
        Debouncer[i].status = STATUS_BUTTON_IDLE;
        
        for (j = 0; j < BUTTON_FILTER_ORDER; ++j)
            Debouncer[i].filter[j] = 0;
    }
}

void RegisterClickFunction(CallBackFunction __function)
{
    ButtonHook.ClickFunc = __function;
}

void RegisterDoubleClickFunction(CallBackFunction __function)
{
    ButtonHook.DoubleClickFunc = __function;
}

void RegisterLongPressFunction(CallBackFunction __function)
{
    ButtonHook.LongPressFunc = __function;
}

static uint8_t readButton(uint8_t idxbtn)
{
    return GPIO_ReadInputDataBit(ButtonConfigs[idxbtn].Port, ButtonConfigs[idxbtn].Pin) ^ Debouncer[idxbtn].pullType;
}

void ButtonProcess(void)
{
    uint8_t i, j;
    uint8_t sum = 0;
    BUTTON_STATUS btnSts = STATUS_BUTTON_IDLE;

    for (i = 0; i < MULTI_BUTTON; ++i)
    {
        sum = 0;
        btnSts = STATUS_BUTTON_IDLE;

        Debouncer[i].filter[Debouncer[i].index] = readButton(i);

        for (j = 0; j < BUTTON_FILTER_ORDER; ++j)
        {
            sum = sum + Debouncer[i].filter[j];
        }

        Debouncer[i].index++;

        if (Debouncer[i].index >= BUTTON_FILTER_ORDER)
        {
            Debouncer[i].index = 0;
        }

        btnSts = ((sum == BUTTON_FILTER_ORDER) ? STATUS_BUTTON_PRESS : STATUS_BUTTON_RELEASE);

        if (Debouncer[i].status != btnSts)
        {
            Debouncer[i].status = btnSts;
            if (Debouncer[i].status == STATUS_BUTTON_PRESS) Debouncer[i].pressTime = 0;
            else Debouncer[i].releaseTime = 0;
        }
        else
        {
            if (Debouncer[i].status == STATUS_BUTTON_PRESS && Debouncer[i].pressTime < 0xFFFFFFFFU)
            {
                Debouncer[i].pressTime++;
            }
            else if (Debouncer[i].status == STATUS_BUTTON_RELEASE && Debouncer[i].releaseTime < 0xFFU)
            {
                Debouncer[i].releaseTime++;
            }
        }
    }
}

uint8_t ButtonPress(uint8_t idxbtn)
{
    return (Debouncer[idxbtn].status == STATUS_BUTTON_PRESS) ? 0xFF : 0x00;
}

uint8_t ButtonRelease(uint8_t idxbtn)
{
    return (Debouncer[idxbtn].status == STATUS_BUTTON_RELEASE) ? 0xFF : 0x00;
}

BUTTON_STATUS ButtonClick(uint8_t idxbtn)
{
    if (ButtonPress(idxbtn) && Debouncer[idxbtn].pressTime - Debouncer[idxbtn].lastPressTime > BUTTON_LONG_TIME)
    {
        Debouncer[idxbtn].lastPressTime = Debouncer[idxbtn].pressTime;

        if (ButtonHook.LongPressFunc)
            ButtonHook.LongPressFunc(idxbtn);
        
        return STATUS_BUTTON_LONG_PRESS;
    }

    if (ButtonRelease(idxbtn))
    {
        if (Debouncer[idxbtn].pressTime > BUTTON_DEBOUNCE_TIME && Debouncer[idxbtn].pressTime < BUTTON_LONG_TIME)
        {
            Debouncer[idxbtn].nClicks++;
            Debouncer[idxbtn].pressTime = 0;
        }

        if (Debouncer[idxbtn].releaseTime > BUTTON_RELEASE_TIME)
        {
            if (Debouncer[idxbtn].nClicks == 1)
            {
                Debouncer[idxbtn].nClicks = 0;

                if (ButtonHook.ClickFunc)
                    ButtonHook.ClickFunc(idxbtn);

                return STATUS_BUTTON_CLICK;
            }
            else if (Debouncer[idxbtn].nClicks == 2)
            {
                Debouncer[idxbtn].nClicks = 0;

                if (ButtonHook.DoubleClickFunc)
                    ButtonHook.DoubleClickFunc(idxbtn);
                
                return STATUS_BUTTON_DOUBLE_CLICK;
            }

            Debouncer[idxbtn].nClicks = 0;
            Debouncer[idxbtn].pressTime = 0;
            Debouncer[idxbtn].lastPressTime = 0;
            return STATUS_BUTTON_IDLE;
        }
    }

    return STATUS_BUTTON_IDLE;
}
