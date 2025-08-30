#ifndef __ILI9341_H__
#define __ILI9341_H__

#include <stdint.h>
#include <stdbool.h>
#include <stm32_driver_gpio.h>
#include <stm32_driver_spi.h>

// Screen resolution
#define ILI9341_MAX_HORIZONTAL   240
#define ILI9341_MAX_VERTICAL     320

// ILI9341 Commands
#define ILI9341_CMD_NOP              0x00
#define ILI9341_CMD_SOFT_RESET       0x01
#define ILI9341_CMD_SLEEP_OUT        0x11
#define ILI9341_CMD_DISPLAY_OFF      0x28
#define ILI9341_CMD_DISPLAY_ON       0x29
#define ILI9341_CMD_COLUMN_ADDR      0x2A
#define ILI9341_CMD_PAGE_ADDR        0x2B
#define ILI9341_CMD_MEMORY_WRITE     0x2C
#define ILI9341_CMD_MEMORY_ACCESS    0x36
#define ILI9341_CMD_PIXEL_FORMAT     0x3A

// Pixel format
#define ILI9341_PIXEL_FORMAT_16BIT   0x55

// GPIO control macros
#define ILI9341_DC_HIGH      GPIOA->ODR.BITS.ODR9 = 1
#define ILI9341_DC_LOW       GPIOA->ODR.BITS.ODR9 = 0
#define ILI9341_CS_HIGH      GPIOB->ODR.BITS.ODR12 = 1
#define ILI9341_CS_LOW       GPIOB->ODR.BITS.ODR12 = 0

#define LCD_CMD_EOF          0xFF
#define LCD_CMD_DELAY_MS     0xFE   // khác EOF để phân biệt

// API
void ILI9341_Init(void);
void ILI9341_DisplayOn(void);
void ILI9341_DisplayOff(void);
void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch);
void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str);
void ILI9341_DrawSquare(int x, int y, int size, const uint16_t color);
void ILI9341_FillScreen(uint16_t color);

#endif /* __ILI9341_H__ */
