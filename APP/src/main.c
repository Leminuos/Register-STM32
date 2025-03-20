#include "main.h"
#include "lcd_generic.h"
#include "ff.h"

void init(void);
void loop(void);
extern void I2C_Init(I2C_InitHandler* pI2C);
extern void setupHardware(void);
extern void TestLed(void);

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
    ButtonProcess();
}

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

extern const FontTypedef Font_14x15;

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

void HandleButtonEvent(uint8_t evt, uint8_t btn)
{
    switch (btn)
    {
        case 0:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR6 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                GPIOA->ODR.BITS.ODR7 = 0;
                break;
        
                case BUTTON_LONG_PRESS_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                GPIOA->ODR.BITS.ODR7 = 1;
                break;
        
                default:
        
                break;
            }
        }
        break;

        case 1:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR7 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR7 = 0;
                break;
        
                default:
        
                break;
            }
        }
        break;

        case 2:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOB->ODR.BITS.ODR0 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOB->ODR.BITS.ODR0 = 0;
                break;
        
                default:
        
                break;
            }
        }
        break;

        case 4:
        {
            switch (evt)
            {
                case BUTTON_CLICK_EVENT:
                GPIOA->ODR.BITS.ODR6 = 1;
                break;
        
                case BUTTON_RELEASE_EVENT:
                GPIOA->ODR.BITS.ODR6 = 0;
                break;
        
                default:
        
                break;
            }
        }
        break;

        default:
        break;
    }
}

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */

void init(void)
{
    UINT bw;
	FRESULT fr;

    setupHardware();
    SystickConfig(71999);

    TestLed();
    ButtonConfig();
    RegisterButtonEvent(HandleButtonEvent);                            
    //LCD_Create();

    f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */

	fr = f_open(&Fil, "Hello.txt", FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file */
	if (fr == FR_OK) {
		f_write(&Fil, "Hello World", 11, &bw);	/* Write data to the file */
		fr = f_close(&Fil);							/* Close the file */
		if (fr == FR_OK && bw == 11) {		/* Lights green LED if data written well */
			while (1) {
				GPIO_SetBit(GPIOC, GPIO_PIN_13);
				delay(100);
				GPIO_ResetBit(GPIOC, GPIO_PIN_13);
				delay(100);
			}
		}
	}
}

void loop(void)
{
    
}
