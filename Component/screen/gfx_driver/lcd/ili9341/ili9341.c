#include "ili9341.h"

extern void delay(uint16_t mDelay);

// ===== Low-level SPI send =====
void ILI9341_SendCommand(uint8_t cmd)
{
    ILI9341_DC_LOW;
    SPI_WriteByte(SPI2, cmd);
}

void ILI9341_SendData(uint8_t data)
{
    ILI9341_DC_HIGH;
    SPI_WriteByte(SPI2, data);
}

static void ILI9341_SendCommandList(const uint8_t* cmdList)
{
    uint8_t num, cmd, dat;

    while (1)
    {
        cmd = *cmdList++;
        if (cmd == LCD_CMD_EOF) break;

        num = *cmdList++;

        if (cmd == LCD_CMD_DELAY_MS) {
            delay(num);
            continue;
        }

        ILI9341_SendCommand(cmd);
        for (dat = 0; dat < num; dat++) {
            ILI9341_SendData(*cmdList++);
        }
    }
}

// ===== Init sequence for ILI9341 =====
static const uint8_t ili9341_init_seq[] = {
    0xCF, 3, 0x00, 0xC1, 0x30,
    0xED, 4, 0x64, 0x03, 0x12, 0x81,
    0xE8, 3, 0x85, 0x00, 0x78,
    0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
    0xF7, 1, 0x20,
    0xEA, 2, 0x00, 0x00,
    0xC0, 1, 0x23,       // Power control
    0xC1, 1, 0x10,       // Power control
    0xC5, 2, 0x3e, 0x28, // VCOM control
    0xC7, 1, 0x86,       // VCOM control
    0x36, 1, 0x48,       // Memory Access Control
    0x3A, 1, 0x55,       // Pixel Format 16bit
    0xB1, 2, 0x00, 0x18, // Frame Rate Control
    0xB6, 3, 0x08, 0x82, 0x27,
    0x11, 0,             // Sleep Out
    LCD_CMD_DELAY_MS, 120,
    0x29, 0,             // Display ON
    LCD_CMD_EOF
};

// ===== Public Functions =====
void ILI9341_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;

    SPI_Init(SPI2);
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_SetBit(GPIOA, GPIO_PIN_10 | GPIO_PIN_11);

    delay(120);

    ILI9341_CS_LOW;
    ILI9341_SendCommandList(ili9341_init_seq);
    ILI9341_CS_HIGH;

    ILI9341_FillScreen(0xFFFF);
}

void ILI9341_DisplayOn(void)
{
    ILI9341_CS_LOW;
    ILI9341_SendCommand(ILI9341_CMD_DISPLAY_ON);
    ILI9341_CS_HIGH;
}

void ILI9341_DisplayOff(void)
{
    ILI9341_CS_LOW;
    ILI9341_SendCommand(ILI9341_CMD_DISPLAY_OFF);
    ILI9341_CS_HIGH;
}

void ILI9341_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    ILI9341_SendCommand(ILI9341_CMD_COLUMN_ADDR);
    ILI9341_SendData(x1 >> 8);
    ILI9341_SendData(x1 & 0xFF);
    ILI9341_SendData(x2 >> 8);
    ILI9341_SendData(x2 & 0xFF);

    ILI9341_SendCommand(ILI9341_CMD_PAGE_ADDR);
    ILI9341_SendData(y1 >> 8);
    ILI9341_SendData(y1 & 0xFF);
    ILI9341_SendData(y2 >> 8);
    ILI9341_SendData(y2 & 0xFF);

    ILI9341_SendCommand(ILI9341_CMD_MEMORY_WRITE);
}

void ILI9341_DrawSquare(int x, int y, int size, const uint16_t color)
{
    uint32_t i, area = size * size;

    ILI9341_CS_LOW;
    ILI9341_SetWindow(x, y, x + size - 1, y + size - 1);

    for (i = 0; i < area; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }

    ILI9341_CS_HIGH;
}

void ILI9341_FillScreen(uint16_t color)
{
    uint32_t i, area = ILI9341_MAX_HORIZONTAL * ILI9341_MAX_VERTICAL;

    ILI9341_CS_LOW;
    ILI9341_SetWindow(0, 0, ILI9341_MAX_HORIZONTAL - 1, ILI9341_MAX_VERTICAL - 1);

    for (i = 0; i < area; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }

    ILI9341_CS_HIGH;
}
