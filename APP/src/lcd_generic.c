#include "lcd_generic.h"

/*
LED     --------->   A09
SCK     --------->   B13
SDA     --------->   B15
A0      --------->   A05
RESET   --------->   A04
CS      --------->   B12
*/

extern void delay(uint32_t mDelay);

#define ST7735_SendData(data)   SPI_WriteByte(SPI2, data)
#define ST7735_DisplayOn        ST7735_SendCommand(LCD_CMD_SET_DISPLAY_ON)
#define ST7735_DisplayOff       ST7735_SendCommand(LCD_CMD_SET_DISPLAY_OFF)
#define ST7735_DC_HIGH          GPIOA->ODR.BITS.ODR5 = 1
#define ST7735_DC_LOW           GPIOA->ODR.BITS.ODR5 = 0
#define ST7735_CS_HIGH          GPIOB->ODR.BITS.ODR12 = 1 
#define ST7735_CS_LOW           GPIOB->ODR.BITS.ODR12 = 0
#define LCD_CMD_EOF             0xFF
#define LCD_CMD_DELAY_MS        0xFF

static const uint8_t u8InitCmdList[] = {
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

static void ST7735_SendCommand(uint8_t data)
{
    ST7735_DC_LOW;
    SPI_WriteByte(SPI2, data);
    ST7735_DC_HIGH;
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

void ST7735_Test()
{
    uint32_t i = 0;

    ST7735_SendCommand(0x2C);

    for (i = 0; i < 10000; ++i)
    {
        ST7735_SendData(0xF8);
        ST7735_SendData(0);
        delay(20);
    }
}

void LCD_Create()
{
    Disp_Infor infor;
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_9;

    infor.madctlReg = LCD_MASK_ROW_ADDRESS_ORDER | LCD_MASK_ROW_COLUMN_ORDER;
    infor.pixelFormat = LCD_SET_PIXEL_FORMAT_16_BIT;
    infor.inversion = LCD_EXIT_INVERSION;
    infor.dispmode = LCD_NORMAL_MODE;

    SPI_Init(SPI2);
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_SetBit(GPIOA, GPIO_PIN_4 | GPIO_PIN_9);

    ST7735_CS_LOW;

    ST7735_SendCommand(LCD_CMD_SOFT_RESET);
    delay(5);

    ST7735_SendCommand(LCD_CMD_EXIT_SLEEP_MODE);
    delay(5);

    ST7735_SetDispControl(&infor);
    ST7735_SetWindow(0, 0, 160, 128);
    ST7735_SendCommandList(u8InitCmdList);
    ST7735_DisplayOn; //Display on

    ST7735_Test();

    ST7735_CS_HIGH;
}

