//#include "vscode.h"
#include "stm32f103.h"
#include "stm32_driver_i2c.h"
#include "stm32_hal_util.h"

void init(void);
void loop(void);
extern void I2C_Init(I2C_Typedef* I2Cx);
extern void setupHardware(void);
extern void ButtonConfig(void);
extern void TestLed(void);
extern void LEDRGBConfig(void);
extern void TraceInit(void);
extern void USARTInit(void);
extern void EXTIConfig(void);
extern void TimerConfig(void);
extern void USART_Transmiter(USART_Typedef* USARTx, uint8_t data);

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

static uint32_t u32Tick;

uint32_t HAL_GetTick(void)
{
    return u32Tick;
}

void SysTick_Handler(void)
{
    ++u32Tick;
}

extern void delay(uint32_t mDelay);

void delay(uint32_t mDelay)
{
    uint32_t currTime = HAL_GetTick();
    while (HAL_GetTick() - currTime < mDelay);
}

void readMCO(void)
{
    // MCO is System Clock
    RCC->CFGR.BITS.MCO = 0x04;
    
    /* Cau hinh clock port A */
    RCC->APB2ENR.BITS.IOPAEN = 1;
    
    /* Cau hinh MCO */
    GPIOA->CRH.BITS.MODE8 = 0X03;
    GPIOA->CRH.BITS.CNF8 = 0x02;
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}

#define DS3231_ADDRESS          0x68

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    //TraceInit();
    I2C_Init(I2C1);
    uint8_t TransBuffer[] = {0x02, 0x34};
    uint8_t ReceiBuffer[5];
    I2C_Master_Transmitter(I2C1, DS3231_ADDRESS, TransBuffer, 2, 20);
    delay(2000);
    I2C_Master_Receivei(I2C1, DS3231_ADDRESS, 0x01, ReceiBuffer, 2, 20);
    TestLed();
}
 
uint32_t i;
char buff = 'A';

void loop(void)
{
    //for (i = 0; i < 10; ++i) USART_Transmiter(USART1, buff + i);
    //for (i = 0; i < 10000000; ++i);
    TOGGLE_BIT(GPIOA->ODR.BITS.ODR7);
    delay(1000);
}
