#ifndef __OLED__
#define __OLED__

#include "stm32_driver_i2c.h"

#define OLED_INCLUDE_FONT_11x18
#define OLED_INCLUDE_FONT_16x15
#define OLED_ADDRESS                    0x3C
#define MAX_PAGE                        0x08
#define MAX_COL                         0x80
#define MAX_ROW                         0x40

#define OLED_WriteCtrl(__CTRL__)                                        \
        do {                                                            \
            uint8_t buffer[] = {0x00, (__CTRL__)};                      \
            I2C_Master_Transmitter(hi2c, OLED_ADDRESS, buffer, 2, 20);  \
        } while (0)                                                     \

#define OLED_WriteData(__DATA__)                                        \
        do {                                                            \
            uint8_t buffer[] = {0x40, (__DATA__)};                      \
            I2C_Master_Transmitter(hi2c, OLED_ADDRESS, buffer, 2, 20);  \
        } while (0)                                                     \

typedef enum {
    COLOR_BLACK = 0x00,
    COLOR_WHITE = 0xFF
} OLED_COLOR;

typedef struct {
    uint8_t currentX;
    uint8_t currentY;
} OLED_Typedef;

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *const data;
    const uint8_t *const char_width;
} OLED_FontTypedef;

void OLED_Clear(void);
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);
void OLED_UpdateScreen(void);
void OLED_FillData(uint8_t data);
void OLED_Init(I2C_Typedef* I2Cx);
void OLED_SetPoint(uint8_t x, uint8_t y);
void OLED_SetCursor(uint8_t x, uint8_t y);
void OLED_SetContrast(const uint8_t value);
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color);
char OLED_WriteChar(char ch, OLED_FontTypedef Font, OLED_COLOR color);
char OLED_WriteString(char* str, OLED_FontTypedef Font, OLED_COLOR color);   
void OLED_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_COLOR color);
void OLED_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_COLOR color);
void OLED_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, OLED_COLOR color);

#endif /* __OLED__ */
