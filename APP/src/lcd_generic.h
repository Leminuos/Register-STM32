#ifndef __LCD_GENERIC__
#define __LCD_GENERIC__

#include <stdint.h>

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
#define LCD_CMD_SET_ADDRESS_MODE                        0x36    /* Set the data order for transfers from the Host to the display module and from the frame memory to the display device */
#define LCD_CMD_EXIT_IDLE_MODE                          0x38    /* Full color depth is used on the display panel */
#define LCD_CMD_ENTER_IDLE_MODE                         0x39    /* Reduced color depth is used on the display panel */
#define LCD_CMD_SET_PIXEL_FORMAT                        0x3A    /* Defines how many bits per pixel are used in the interface */

#define LCD_MASK_FLIP_VERTICAL                          (1 << 0)    /* This bit flips the image shown on the display device top to bottom. No change is made to the frame memory */
#define LCD_MASK_FLIP_HORIZONTAL                        (1 << 1)    /* This bit flips the image shown on the display device left to right. No change is made to the frame memory */
#define LCD_MASK_DATA_LATCH_DATA_ORDER                  (1 << 2)    /* Display Data Latch Order */
#define LCD_MASK_RGB_ORDER                              (1 << 3)    /* RGB/BGR Order */
#define LCD_MASK_LINE_ADDRESS_ORDER                     (1 << 4)    /* Line Address Order */
#define LCD_MASK_PAGE_COLUMN_ORDER                      (1 << 5)    /* Page/Column Order */
#define LCD_MASK_COLUMN_ADDRESS_ORDER                   (1 << 6)    /* Column Address Order */
#define LCD_MASK_PAGE_ADDRESS_ORDER                     (1 << 7)    /* Page Address Order */

#endif /* __LCD_GENERIC__ */