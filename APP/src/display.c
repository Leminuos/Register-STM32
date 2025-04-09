#include <stdio.h>
#include "oled.h"

extern void delay(uint16_t mDelay);
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
   
    uint32_t start = TIM_GetTimerCount();
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
        end = TIM_GetTimerCount();
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
