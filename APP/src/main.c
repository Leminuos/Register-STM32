//#include "vscode.h"
#include <stdio.h>
#include <string.h>
#include "stm32f103.h"
#include "stm32_driver_i2c.h"
#include "stm32_driver_spi.h"
#include "stm32_hal_util.h"
#include "oled_fonts.h"
#include "oled.h"
#include "spiflash.h"

void init(void);
void loop(void);
extern void I2C_Init(I2C_InitHandler* pI2C);
extern void setupHardware(void);
extern void ButtonConfig(void);
extern void TestLed(void);
extern void LEDRGBConfig(void);
extern void TraceInit(void);
extern void USARTInit(void);
extern void EXTIConfig(void);
extern void TimerConfig(void);

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

uint8_t card_numberbuf[6] = {0};
char* buffer;

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    SPI_Init(SPI2);
    TestLed();

    hi2c1.Instance = I2C1;
    I2C_Init(&hi2c1);
    
    OLED_Init(I2C1);
    TestOled();

    char buf[100];
    char str[] = "buinguyendeptrai";
    uint8_t i = 0;
    i = sizeof(str)/sizeof(str[0]);

    //snprintf(buf, sizeof(buf), "%2X", val);

    W25QXX_WriteData(0x04000, (uint8_t*) str, i);
    W25QXX_ReadData(0x04000, (uint8_t*) buf, i);
    //printf("Card: 0x"); for (i = 0; i < 8; ++i) printf("%X", buf[i]); printf("\r\n");
    printf("%s\r\n", buf);

    W25QXX_GetDeviceID((uint8_t*) buf);
    printf("ID: 0x"); for (i = 0; i < 8; ++i) printf("%X", buf[i]); printf("\r\n");

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

void loop(void)
{
    //printf("123456\r\n");
    //TOGGLE_BIT(GPIOA->ODR.BITS.ODR7);
}
