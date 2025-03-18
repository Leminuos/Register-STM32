#include "main.h"

void init(void);
void loop(void);
extern void I2C_Init(I2C_InitHandler* pI2C);
extern void setupHardware(void);
extern void TestLed(void);
extern void LEDRGBConfig(void);
extern void TraceInit(void);
extern void USARTInit(void);
extern void EXTIConfig(void);
extern void TimerConfig(void);


uint8_t buffcdc[] = "Nguyen dep trai";

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR.BITS.PR1 == 1)
    {
        GPIOA->ODR.BITS.ODR7 = !GPIOA->ODR.BITS.ODR7;
        EXTI->PR.BITS.PR1 = 0;
        NVIC_ClearPendingIRQ(EXTI1_IRQn);
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
    ButtonProcess();
}

extern void delay(uint32_t mDelay);

void delay(uint32_t mDelay)
{
    uint32_t currTime = HAL_GetTick();
    while (HAL_GetTick() - currTime < mDelay);
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}

I2C_InitHandler hi2c1;
I2C_InitHandler hi2c2;

void I2C1_EV_IRQHandler(void)
{
    I2C_EV_IRQHandler(&hi2c1);
}

void I2C1_ER_IRQHandler(void)
{
    I2C_ER_IRQHandler(&hi2c1);
}

void I2C2_EV_IRQHandler(void)
{
    I2C_EV_IRQHandler(&hi2c2);
}

void I2C2_ER_IRQHandler(void)
{
    I2C_ER_IRQHandler(&hi2c2);
}

#define DS3231_ADDRESS          0x68

uint8_t TransBuffer[] = {0x02, 0x48};
uint8_t ReceiBuffer[5];

extern const OLED_FontTypedef Font_14x15;

void TestOled(void)
{
    OLED_Clear();
    OLED_SetCursor(0, 0);
    OLED_AutoNewline(ENABLE);
    OLED_WriteStringVIE("Bùi Nguyên - Omega Von Pei - Bùi Văn Nguyên - 28 - 04 - 2001", Font_14x15, COLOR_WHITE);
    OLED_UpdateScreen();
}

void OLED_TestFPS() {
    OLED_FillData(COLOR_BLACK);
   
    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    int fps = 0;
    char message[] = "ABCDEFGHIJK";
   
    OLED_SetCursor(2,0);
    OLED_WriteString("Testing...", Font_11x18, COLOR_WHITE);
    OLED_SetCursor(2, 18*2);
    OLED_WriteString("0123456789A", Font_11x18, COLOR_WHITE);
   
    do {
        OLED_SetCursor(2, 18);
        OLED_WriteString(message, Font_11x18, COLOR_WHITE);
        OLED_UpdateScreen();
       
        char ch = message[0];
        memmove(message, message+1, sizeof(message)-2);
        message[sizeof(message)-2] = ch;

        fps++;
        end = HAL_GetTick();
    } while((end - start) < 5000);
   
    delay(2000);

    char buff[64];
    fps = (float)fps / ((end - start) / 1000.0);
    snprintf(buff, sizeof(buff), "~%d FPS", fps);
   
    OLED_FillData(COLOR_BLACK);
    OLED_SetCursor(2, 2);
    OLED_WriteString(buff, Font_11x18, COLOR_WHITE);
    OLED_UpdateScreen();
}

void OLED_TestRectangleFill() {
  OLED_FillRectangle(31, 1, 65, 35, COLOR_WHITE);
  OLED_FillRectangle(10, 45, 70, 60, COLOR_WHITE);
  OLED_FillRectangle(75, 10, 100, 45, COLOR_WHITE);
  OLED_FillRectangle(55, 30, 80, 55, COLOR_BLACK);
  OLED_UpdateScreen();
}

extern const unsigned char garfield_128x64[];
extern const unsigned char github_logo_64x64[];

void OLED_TestDrawBitmap()
{
    OLED_FillData(COLOR_WHITE);
    OLED_DrawBitmap(0,0,garfield_128x64,128,64, COLOR_BLACK);
    OLED_UpdateScreen();
    delay(3000);
    OLED_FillData(COLOR_BLACK);
    OLED_DrawBitmap(32,0, github_logo_64x64,64,64, COLOR_WHITE);
    OLED_UpdateScreen();
    delay(3000);
    OLED_FillData(COLOR_WHITE);
    OLED_DrawBitmap(32,0, github_logo_64x64,64,64, COLOR_BLACK);
    OLED_UpdateScreen();
}

void BlinkBlue(uint8_t idxbtn)
{;
    if (idxbtn == 1)
        TOGGLE_BIT(GPIOA->ODR.BITS.ODR6);
}

void BlinkRed(uint8_t idxbtn)
{
    if (idxbtn == 1)
        TOGGLE_BIT(GPIOA->ODR.BITS.ODR7);
}

void BlinkGreen(uint8_t idxbtn)
{
    if (idxbtn == 1)
        TOGGLE_BIT(GPIOB->ODR.BITS.ODR0);
}

void init(void)
{
    // #define RESET_HANDLER_ADDR       (*((volatile uint32_t*) (0x08002004U)))
    // SCB->VTOR = (uint32_t)0x08002000;
    // void (*reset_handler)(void) = (void (*)(void)) RESET_HANDLER_ADDR;
    // reset_handler();
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    //SPI_Init(SPI2);
    TestLed();
    // ButtonConfig();
    // RegisterClickFunction(BlinkBlue);
    // RegisterDoubleClickFunction(BlinkRed);
    // RegisterLongPressFunction(BlinkGreen);

    // hi2c1.Instance = I2C1;
    // I2C_Init(&hi2c1);
    
    // OLED_Init(I2C1);
    // TestOled();

    USB_PowerOnReset();

    //char buf[100];
    //snprintf(buf, sizeof(buf), "%2X", val);

    // hi2c2.Instance = I2C2;
    // hi2c2.SlaveMode = ADDR_7_BIT;
    // hi2c2.OwnAddress1 = 0x48;
    // I2C_Init(&hi2c2);

    // uint16_t RxSize = sizeof(ReceiBuffer) / sizeof(ReceiBuffer[0]);
    // (void) RxSize;

    // I2C_Slave_Receiver_IT(&hi2c2, ReceiBuffer, RxSize);
    // I2C_Master_Transmitter_IT(&hi2c1, 0x48, TransBuffer, 2);

    // //I2C_Master_Transmitter_IT(&hi2c1, DS3231_ADDRESS, TransBuffer, 2);
    // //I2C_Master_Transmitter(I2C1, DS3231_ADDRESS, TransBuffer, 2, 20);
    // delay(2000);
    // I2C_Slave_Transmitter_IT(&hi2c2, ReceiBuffer, RxSize);
    // //I2C_Master_Receiver(I2C1, DS3231_ADDRESS, 0x01, ReceiBuffer, 2, 20);
    // //I2C_Master_Receiver_IT(&hi2c1, DS3231_ADDRESS, ReceiBuffer, 5);
    // I2C_Master_Receiver_IT(&hi2c1, 0x48, ReceiBuffer, 2);
}

// uint8_t i = 0;

void loop(void)
{
    //printf("123456\r\n");
    // for (i = 0; i < MULTI_BUTTON; ++i)
    //     ButtonClick(i);
//    if (USB->ISTR.BITS.RESET)
//    {
//        USB_ResetCallBack();
//    }

    GPIOC->ODR.BITS.ODR13 = 0;
    CDC_Transmit("Anh em minh la mot gia dinh\r\n");
    delay(1000);
    GPIOC->ODR.BITS.ODR13 = 1;
    delay(1000);
}
