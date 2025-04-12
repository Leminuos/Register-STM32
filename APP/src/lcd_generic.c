#include "lcd_generic.h"

/*
LED     --------->   A09
SCK     --------->   B13
SDA     --------->   B15
A0      --------->   A05
RESET   --------->   A04
CS      --------->   B12
*/

typedef struct {
    const FontTypedef*    font;
    uint8_t         font_color[2];
    uint8_t         text_highlight[2];
} TextEditor;

typedef struct {
    uint8_t madctlReg;
    uint8_t pixelFormat;
    bool    inversion;
    bool    dispmode;
} Disp_Infor;

extern void delay(uint16_t mDelay);

static TextEditor Fonts;

static const uint8_t u8PanelCmdList[] = {
//  Command     Length      Data
    0xB1,       0x03,       0x01, 0x2C, 0x2D,                       // Frame Rate Control (In normal mode/ Full colors)
    0xB2,       0x03,       0x01, 0x2C, 0x2D,                       // Frame Rate Control (In Idle mode/ 8-colors)
    0xB3,       0x06,       0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,     // Frame Rate Control (In Partial mode/ full colors)
    0xB4,       0x01,       0x07,                                   // Display Inversion Control
    0xC0,       0x03,       0xA2, 0x02, 0x84,                       // Power Control 1
    0xC1,       0x01,       0xC5,                                   // Power Control 2
    0xC2,       0x02,       0x0A, 0x00,                             // Power Control 3 (in Normal mode/ Full colors)
    0xC3,       0x02,       0x8A, 0x2A,                             // Power Control 4 (in Idle mode/ 8-colors)
    0xC4,       0x02,       0x8A, 0xEE,                             // Power Control 5 (in Partial mode/ full-colors)
    0xC5,       0x01,       0x0E,                                   // VCOM Control 1
    0xE0,       0x10,       0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10,     //Gamma adjustment(+ polarity)
    0xE1,       0x10,       0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,     //Gamma adjustment(- polarity)
    LCD_CMD_DELAY_MS, LCD_CMD_EOF
};

static void ST7735_SendCommand(uint8_t cmd)
{
    ST7735_DC_LOW;
    SPI_WriteByte(SPI2, cmd);
}

static void ST7735_SendData(uint8_t data)
{
    ST7735_DC_HIGH;
    SPI_WriteByte(SPI2, data);
}

static void ST7735_SendCommandList(const uint8_t* cmdList)
{
    uint8_t dat = 0;
    uint8_t cmd = 0;
    uint8_t num = 0;

    while (1)
    {
        cmd = *cmdList++;
        num = *cmdList++;

        if (cmd == LCD_CMD_DELAY_MS)  {
            if (num == LCD_CMD_EOF)         /* end of list */
                break;
            else                            /* delay in 10 ms units*/
                delay((uint32_t)(num*10));
        }
        else {
            ST7735_SendCommand(cmd);

            for (dat = 0; dat < num; ++dat)
                ST7735_SendData(*cmdList++);
        }
    }
}

void ST7735_SetDispControl(Disp_Infor *infor)
{
    ST7735_SendCommand(LCD_CMD_SET_MADCTL);  //Memory Data Access Control           
    ST7735_SendData(infor->madctlReg);

    ST7735_SendCommand(LCD_CMD_SET_PIXEL_FORMAT); //Interface Pixel Format
    ST7735_SendData(infor->pixelFormat);

    infor->inversion ? ST7735_SendCommand(LCD_CMD_ENTER_INVERT_MODE) : ST7735_SendCommand(LCD_CMD_EXIT_INVERT_MODE);

    infor->dispmode ? ST7735_SendCommand(LCD_CMD_ENTER_NORMAL_MODE) : ST7735_SendCommand(LCD_CMD_ENTER_PARTIAL_MODE);
}

void ST7735_DisplayOn()
{
    ST7735_SELECT;
    ST7735_SendCommand(LCD_CMD_SET_DISPLAY_ON);
    ST7735_UNSELECT;
}
void ST7735_DisplayOff()
{
    ST7735_SELECT;
    ST7735_SendCommand(LCD_CMD_SET_DISPLAY_OFF);
    ST7735_SELECT;
}

void ST7735_SetWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    ST7735_SendCommand(LCD_CMD_SET_COLUMN_ADDRESS); //Column address set
    ST7735_SendData(0x00);
    ST7735_SendData(x1);
    ST7735_SendData(0x00);
    ST7735_SendData(x2);
    
    ST7735_SendCommand(LCD_CMD_SET_PAGE_ADDRESS);   //Row address set
    ST7735_SendData(0x00);
    ST7735_SendData(y1);
    ST7735_SendData(0x00);
    ST7735_SendData(y2);

    ST7735_SendCommand(LCD_CMD_WRITE_MEMORY_START);
}

void ST7735_Reset()
{
    ST7735_SendCommand(LCD_CMD_SOFT_RESET);
    delay(5);

    ST7735_SendCommand(LCD_CMD_EXIT_SLEEP_MODE);
    delay(5);
}

void ST7735_SetFont(const FontTypedef* font, uint8_t* color, uint8_t* hightlight)
{
    Fonts.font = font;
    Fonts.font_color[0] = color[0];
    Fonts.font_color[1] = color[1];
    Fonts.text_highlight[0] = hightlight[0];
    Fonts.text_highlight[1] = hightlight[1];
}

void ST7735_WriteChar(uint8_t x, uint8_t y, char ch)
{
    uint8_t i, j;
    uint16_t word = 0;

    if (((x + Fonts.font->width) > ST7735_MAX_HORIZONTAL) || ((y + Fonts.font->height) > ST7735_MAX_VERTICAL))
        return;

    ST7735_SELECT;

    ST7735_SetWindow(x, y, x + Fonts.font->width - 1, y + Fonts.font->height - 1);

    for (i = 0; i < Fonts.font->height; ++i)
    {
        word = Fonts.font->data[(ch - 32) * Fonts.font->height + i];

        for (j = 0; j < Fonts.font->width; ++j)
        {
            if ((word << j) & 0x8000)
            {
                ST7735_SendData(Fonts.font_color[0]);
                ST7735_SendData(Fonts.font_color[1]);
            }
            else
            {
                ST7735_SendData(Fonts.text_highlight[0]);
                ST7735_SendData(Fonts.text_highlight[1]);
            }
        }
    }

    ST7735_UNSELECT;
}

void ST7735_WriteString(uint8_t x, uint8_t y, const char* str)
{
    while (*str)
    {
        if (x + Fonts.font->width >= ST7735_MAX_HORIZONTAL)
        {
            x = 0;
            y = y + Fonts.font->height;

            if (y > ST7735_MAX_VERTICAL) break;

            // skip spaces in the beginning of the new line
            if (*str == ' ')
            {
                ++str;
                continue;
            }
        }
        ST7735_WriteChar(x, y, *str);
        x = x + Fonts.font->width;
        ++str;
    }
}

void ST7735_DrawSquare(int x, int y, int size, const uint8_t* color)
{
    uint32_t i = 0, area = 0;

    ST7735_SELECT;

    ST7735_SetWindow(x, y, x + size, y + size);

    area = size * size - 1;

    for (i = 0; i < area; ++i)
    {
        ST7735_SendData(color[0]);
        ST7735_SendData(color[1]);
    }

    ST7735_UNSELECT;
}

void ST7735_FillScreen()
{
    uint32_t i = 0;
    
    ST7735_SELECT;

    ST7735_SetWindow(0, 0, 160, 128);

    for (i = 0; i < 20480; ++i)
    {
        ST7735_SendData(0xFF);
        ST7735_SendData(0xFF);
    }
    
    ST7735_UNSELECT;
}

void LCD_Create()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_9;

    SPI_Init(SPI2);
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_SetBit(GPIOA, GPIO_PIN_4 | GPIO_PIN_9);

    ST7735_SELECT;
    ST7735_Reset();
    ST7735_SendCommandList(u8PanelCmdList);
    ST7735_SetDispControl(&(Disp_Infor){
        LCD_MASK_ROW_ADDRESS_ORDER | LCD_MASK_ROW_COLUMN_ORDER,
        LCD_SET_PIXEL_FORMAT_16_BIT,
        LCD_EXIT_INVERSION,
        LCD_NORMAL_MODE
    });
    ST7735_DisplayOn(); //Display on
    ST7735_FillScreen();
    ST7735_UNSELECT;
}

