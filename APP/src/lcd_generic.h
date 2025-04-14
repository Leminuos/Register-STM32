#ifndef __LCD_GENERIC__
#define __LCD_GENERIC__

#include <stdint.h>
#include <stm32_driver_gpio.h>
#include <stm32_driver_spi.h>
#include <stdbool.h>
#include "fonts.h"

#define ST7735_MAX_HORIZONTAL                           160
#define ST7735_MAX_VERTICAL                             128

#define LCD_CMD_NOP                                     0x00    /* No Operation */
#define LCD_CMD_SOFT_RESET                              0x01    /* Software Reset */
#define LCD_CMD_GET_POWER_MODE                          0x0A    /* Get the current power mode */
#define LCD_CMD_GET_ADDRESS_MODE                        0x0B    /* Get the data order for transfers from the Host to the display module and from the frame memory to the display device */
#define LCD_CMD_GET_PIXEL_FORMAT                        0x0C    /* Get the current pixel format */
#define LCD_CMD_GET_DISPLAY_MODE                        0x0D    /* Get the current display mode from the peripheral */
#define LCD_CMD_GET_SIGNAL_MODE                         0x0E    /* Get display module signaling mode */
#define LCD_CMD_ENTER_SLEEP_MODE                        0x10    /* Power for the display panel is off */
#define LCD_CMD_EXIT_SLEEP_MODE                         0x11    /* Power for the display panel is on */
#define LCD_CMD_ENTER_PARTIAL_MODE                      0x12    /* Part of the display area is used for image display */
#define LCD_CMD_ENTER_NORMAL_MODE                       0x13    /* The whole display area is used for image display */
#define LCD_CMD_EXIT_INVERT_MODE                        0x20    /* Displayed image colors are not inverted */
#define LCD_CMD_ENTER_INVERT_MODE                       0x21    /* Displayed image colors are inverted */
#define LCD_CMD_SET_GAMMA_CURVE                         0x26    /* Selects the gamma curve used by the display device */
#define LCD_CMD_SET_DISPLAY_OFF                         0x28    /* Blanks the display device */
#define LCD_CMD_SET_DISPLAY_ON                          0x29    /* Show the image on the display device */
#define LCD_CMD_SET_COLUMN_ADDRESS                      0x2A    /* Set the column extent */
#define LCD_CMD_SET_PAGE_ADDRESS                        0x2B    /* Set the page extent */
#define LCD_CMD_WRITE_MEMORY_START                      0x2C    /* Transfer image data from the Host Processor to the peripheral starting at the location provided by set_column_address and set_page_address */
#define LCD_CMD_READ_MEMORY_START                       0x2E    /* Transfer image data from the peripheral to the Host Processor interface starting at the location provided by set_column_address and set_page_address */
#define LCD_CMD_SET_PARTIAL_ROWS                        0x30    /* Defines the number of rows in the partial display area on the display device */
#define LCD_CMD_SET_TEAR_OFF                            0x34    /* Synchronization information is not sent from the display module to the host processor */
#define LCD_CMD_SET_TEAR_ON                             0x35    /* Synchronization information is sent from the display module to the host processor at the start of VFP */
#define LCD_CMD_SET_MADCTL                              0x36    /* Set the data order for transfers from the Host to the display module and from the frame memory to the display device */
#define LCD_CMD_EXIT_IDLE_MODE                          0x38    /* Full color depth is used on the display panel */
#define LCD_CMD_ENTER_IDLE_MODE                         0x39    /* Reduced color depth is used on the display panel */
#define LCD_CMD_SET_PIXEL_FORMAT                        0x3A    /* Defines how many bits per pixel are used in the interface */

#define LCD_MASK_HORIZONTAL_REFRESH_ORDER               (1 << 2)    /* Horizontal Refresh Order  */
#define LCD_MASK_RGB_ORDER                              (1 << 3)    /* RGB/BGR Order */
#define LCD_MASK_VERTICAL_REFRESH_ORDER                 (1 << 4)    /* Vertical Refresh Order */
#define LCD_MASK_ROW_COLUMN_ORDER                       (1 << 5)    /* Row/Column Order */
#define LCD_MASK_COLUMN_ADDRESS_ORDER                   (1 << 6)    /* Column Address Order */
#define LCD_MASK_ROW_ADDRESS_ORDER                      (1 << 7)    /* Row Address Order */

#define LCD_SET_PIXEL_FORMAT_12_BIT                     0x03
#define LCD_SET_PIXEL_FORMAT_16_BIT                     0x05
#define LCD_SET_PIXEL_FORMAT_18_BIT                     0x06

#define LCD_NORMAL_MODE                                 true
#define LCD_PARTIAL_MODE                                false

#define LCD_ENTER_INVERSION                             true
#define LCD_EXIT_INVERSION                              false

#define ST7735_DC_HIGH                                  GPIOA->ODR.BITS.ODR5 = 1
#define ST7735_DC_LOW                                   GPIOA->ODR.BITS.ODR5 = 0
#define ST7735_UNSELECT                                 GPIOB->ODR.BITS.ODR12 = 1 
#define ST7735_SELECT                                   GPIOB->ODR.BITS.ODR12 = 0
#define LCD_CMD_EOF                                     0xFF
#define LCD_CMD_DELAY_MS                                0xFF

extern void LCD_Create(void);
extern void ST7735_Test(void);
extern void ST7735_DisplayOn(void);
extern void ST7735_DisplayOff(void);
extern void ST7735_WriteChar(uint8_t x, uint8_t y, char ch);
extern uint8_t ST7735_WriteString(uint8_t x, uint8_t y, const char* str);
extern void ST7735_DrawSquare(int x, int y, int size, const uint8_t* color);
extern void ST7735_SetWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
extern void ST7735_SetFont(const FontTypedef* font, uint8_t* color, uint8_t* hightlight);

#endif /* __LCD_GENERIC__ */
