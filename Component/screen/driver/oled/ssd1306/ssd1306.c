#include "ssd1306.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

static OLED_Typedef OLED;
static I2C_Typedef* hi2c;
static uint8_t      GDDRAM[8][128];

extern void delay(uint16_t mDelay);

void OLED_FillData(uint8_t data)
{
    memset(GDDRAM, data, sizeof(GDDRAM)/sizeof(GDDRAM[0][0]));
}

void OLED_AutoNewline(FunctionalState state)
{
    OLED.autoNewLine = state;
}

void OLED_UpdateScreen(void)
{
    uint8_t page, col;

    for (page = 0; page < MAX_PAGE; ++page)
    {
        OLED_WriteCtrl(0xB0 + page);             // page0-page7
        OLED_WriteCtrl(0x00);                    // low column start address
        OLED_WriteCtrl(0x10);                    // high column start address
        
        for (col = 0; col < MAX_COL; ++col)
        {
            OLED_WriteData(GDDRAM[page][col]);
        }
    }
}

void OLED_SetPoint(uint8_t x, uint8_t y)
{
    OLED_WriteCtrl(0xB0 + y);                    // page0-page7
    OLED_WriteCtrl((x & 0x0F) | 0x00);           // low column start address
    OLED_WriteCtrl(((x & 0xF0) >> 4) | 0x10);    // high column start address
}

void OLED_SetCursor(uint8_t x, uint8_t y)
{
    OLED.currentX = x;
    OLED.currentY = y;
}

void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color)
{
    if (x > MAX_COL || y > MAX_ROW) return;

    if (color == COLOR_WHITE)
        GDDRAM[y / 8][x] |= 1 << (y % 8);
    else
        GDDRAM[y / 8][x] &= ~(1 << (y % 8));
}

char OLED_WriteChar(char ch, FontTypedef Font, OLED_COLOR color)
{
    uint8_t i, j;
    uint16_t block;

    if (ch < 32U || ch > 126U) return 0;

    if (OLED.currentX + Font.width > MAX_COL || OLED.currentY + Font.height > MAX_ROW)
        return 0;

    for (i = 0; i < Font.height; ++i)
    {
        block = Font.data[(ch - 32U) * Font.height + i];

        for (j = 0; j < Font.width; ++j)
        {
            if ((block << j) & 0x8000)
                OLED_DrawPixel(OLED.currentX + j, OLED.currentY + i, (OLED_COLOR) color);
            else
                OLED_DrawPixel(OLED.currentX + j, OLED.currentY + i, (OLED_COLOR) !color);
        }
    }

    OLED.currentX = OLED.currentX + (Font.char_width ? Font.char_width[ch - 32] : Font.width);
    
    return ch;
}

char OLED_WriteString(char* str, FontTypedef Font, OLED_COLOR color)
{
    while (*str)
    {
        if (OLED_WriteChar(*str, Font, color) != (*str))
        {
            return *str;
        }

        ++str;
    }

    return *str;
}

#ifdef SUPPORT_VIE
uint32_t UTF8_Decode(char* utf8Ch, uint8_t* offset)
{
    uint32_t utf8Val = 0x000000000U;
    uint8_t header = 0xF0 & utf8Ch[0];

    if((*utf8Ch) < 128U)    // ascii
    {
        *offset = 1;
        utf8Val = (uint32_t) *utf8Ch;
        return utf8Val;
    }
    
    if (header == 0xC0)     // 2 byte
    {
        *offset = 2;
        utf8Val = ((uint32_t) utf8Ch[1]);
        utf8Val |= ((uint32_t) utf8Ch[0]) << 8;
    }
    else if (header == 0xE0)        // 3 byte
    {
        *offset = 3;
        utf8Val = ((uint32_t) utf8Ch[2]);
        utf8Val |= ((uint32_t) utf8Ch[1]) << 8;
        utf8Val |= ((uint32_t) utf8Ch[0]) << 16;
    }
    else if (header == 0xF0)        // 4 byte
    {
        *offset = 4;
        utf8Val = ((uint32_t) utf8Ch[3]);
        utf8Val |= ((uint32_t) utf8Ch[2]) << 8;
        utf8Val |= ((uint32_t) utf8Ch[1]) << 16;
        utf8Val |= ((uint32_t) utf8Ch[0]) << 24;
    }
    
    return utf8Val;
}

extern uint32_t UTF8_table[];

uint32_t UTF8_GetCode(uint32_t utf8Val)
{
    uint32_t idx = 0;
    uint32_t size = 145;

    for (idx = 0; idx < size; ++idx)
        if (UTF8_table[idx] == utf8Val)
            break;

    return idx;
}

uint8_t OLED_WriteCharVIE(char* ch, FontTypedef Font, OLED_COLOR color)
{
    uint8_t i, j, offset;
    uint16_t block;
    uint32_t idx, utf8Val;

    if (OLED.currentY + Font.height > MAX_ROW)
        return 0;

    if (OLED.currentX + Font.width > MAX_COL && OLED.autoNewLine == DISABLE)
        return 0;

    if (OLED.currentX + Font.width > MAX_COL && OLED.autoNewLine == ENABLE)
    {
        OLED.currentX = 0;
        OLED.currentY = OLED.currentY + Font.height;
    }

    utf8Val = UTF8_Decode(ch, &offset);
    idx = UTF8_GetCode(utf8Val);

    for (i = 0; i < Font.height; ++i)
    {
        block = Font.data[idx * Font.height + i];

        for (j = 0; j < Font.width; ++j)
        {
            if ((block << j) & 0x8000)
                OLED_DrawPixel(OLED.currentX + j, OLED.currentY + i, (OLED_COLOR) color);
            else
                OLED_DrawPixel(OLED.currentX + j, OLED.currentY + i, (OLED_COLOR) !color);
        }
    }

    OLED.currentX = OLED.currentX + (Font.char_width ? Font.char_width[idx] : Font.width);
    
    return offset;
}

char OLED_WriteStringVIE(char* str, FontTypedef Font, OLED_COLOR color)
{
    uint8_t offset = 0;

    while (*str)
    {
        offset = OLED_WriteCharVIE(str, Font, color);

        if (offset == 0) return *str;

        str = str + offset;
    }

    return *str;
}

#endif /* SUPPORT_VIE */

void OLED_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, OLED_COLOR color)
{
    int16_t byteWidth = (w + 7) / 8;
    uint8_t i, j;
    uint8_t byte = 0;

    if (x >= MAX_COL || y >= MAX_ROW)
        return;

    for (j = 0; j < h; j++, y++) {
        for (i = 0; i < w; i++) {
            if (i & 7)
                byte <<= 1;
            else
                byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));

            if (byte & 0x80)
                OLED_DrawPixel(x + i, y, color);
        }
    }

    return;
}

/* Draw a filled rectangle */
void OLED_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_COLOR color)
{
    uint8_t x, y;
    uint8_t x_start = ((x1<=x2) ? x1 : x2);
    uint8_t x_end   = ((x1<=x2) ? x2 : x1);
    uint8_t y_start = ((y1<=y2) ? y1 : y2);
    uint8_t y_end   = ((y1<=y2) ? y2 : y1);

    for (y = y_start; (y<= y_end) && (y < MAX_ROW); y++) {
        for (x = x_start; (x<= x_end) && (x < MAX_COL); x++) {
            OLED_DrawPixel(x, y, color);
        }
    }

    return;
}

/* Draw line by Bresenhem's algorithm */
void OLED_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_COLOR color)
{
    int32_t deltaX = abs(x2 - x1);
    int32_t deltaY = abs(y2 - y1);
    int32_t signX = ((x1 < x2) ? 1 : -1);
    int32_t signY = ((y1 < y2) ? 1 : -1);
    int32_t error = deltaX - deltaY;
    int32_t error2;
    
    OLED_DrawPixel(x2, y2, color);

    while((x1 != x2) || (y1 != y2)) {
        OLED_DrawPixel(x1, y1, color);
        error2 = error * 2;
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        
        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
    return;
}

void OLED_SetContrast(const uint8_t value)
{
    OLED_WriteCtrl(0x81);    /* Set Contrast Control */
    OLED_WriteCtrl(value);
}

void OLED_Clear(void)
{
    OLED_FillData(COLOR_BLACK);
    OLED_UpdateScreen();
}

void OLED_DisplayOn(void)
{
    OLED_WriteCtrl(0x8D);    /* Enable charge pump regulator */
    OLED_WriteCtrl(0x14);
    OLED_WriteCtrl(0xAF);    /* Turn on OLED panel display */
}

void OLED_DisplayOff(void)
{
    OLED_WriteCtrl(0x8D);    /* Enable charge pump regulator */
    OLED_WriteCtrl(0x10);
    OLED_WriteCtrl(0xAE);    /* Turn off OLED panel display */
}

void OLED_Init(I2C_Typedef* I2Cx)
{
    hi2c = I2Cx;
    
    delay(100);             // 100ms

    /* Init Oled */
    OLED_WriteCtrl(0xAE);    /* Turn off OLED panel display */
    OLED_WriteCtrl(0xA8);    /* Set MUX Ratio */
    OLED_WriteCtrl(0x3F);
    OLED_WriteCtrl(0xD3);    /* Set Display Offset */
    OLED_WriteCtrl(0x00);
    OLED_WriteCtrl(0x40);    /* Set Display Start Line */
    OLED_WriteCtrl(0xA1);    /* Set Segment re-map 0 to 127 */
    OLED_WriteCtrl(0xC8);    /* Set COM Output Scan Direction */
    OLED_WriteCtrl(0xDA);    /* Set COM Pins hardware configuration */
    OLED_WriteCtrl(0x12);
    OLED_WriteCtrl(0x20);    /* Set Memory Addressing Mode */
    OLED_WriteCtrl(0x10);
    OLED_WriteCtrl(0x00);    /* Set low column address */
	OLED_WriteCtrl(0x10);    /* Set high column address */
    OLED_WriteCtrl(0x81);    /* Set Contrast Control */
    OLED_WriteCtrl(0x7F);
    OLED_WriteCtrl(0xA4);    /* Disable Entire Display On */
    OLED_WriteCtrl(0xA6);    /* Set Normal Display */
    OLED_WriteCtrl(0xD5);    /* Set Osc Frequency */
    OLED_WriteCtrl(0x80);
    OLED_WriteCtrl(0xDB);    /* Set VCOMH Deselect Level */
	OLED_WriteCtrl(0x20);
    OLED_WriteCtrl(0x8D);    /* Enable charge pump regulator */
    OLED_WriteCtrl(0x14);
    OLED_WriteCtrl(0xAF);    /* Turn on OLED panel display */

    #if 0
    /* ==================================================================================
    
    Fundamental Command Table
    - 0x81:         Set Contrast Control
    - 0xA4/0xA5:    Entire Display ON
    - 0xA6/0xA7:    Set Normal/Inverse Display
    - 0xAE/0xAF:    Set Display ON/OFF

    =====================================================================================*/
    
    OLED_WriteCtrl(0xAE);    /* Turn off OLED panel display */

    OLED_WriteCtrl(0x81);    /* ==========================================================*/
    OLED_WriteCtrl(0xFF);    /* Set contrast control register
                               Double byte command to select 1 out of 256 contrast steps
                               (RESET = 7Fh).
                            =============================================================*/

    OLED_WriteCtrl(0xA6);    /* ==========================================================
                               Normal Display
                               0 in RAM: OFF in display panel
                               1 in RAM: ON in display panel
                            =============================================================*/

    OLED_WriteCtrl(0xA4);    /* ==========================================================
                               Entire Display ON
                               0xA4: Display output follows RAM content.
                               0xA5: Force the entire display to be "ON".
                            =============================================================*/

    /* ==================================================================================
    
    Addressing Setting Command Table
    - 0x00~0x0F:    Set Lower Column Start Address for Page Addressing Mode
    - 0x10~0x1F:    Set Higher Column Start Address for Page Addressing Mode
    - 0x20:         Set Memory Addressing Mode 
    - 0x21:         Set Column Address
    - 0x22:         Set Page Address
    - 0xB0~0xB7:    Set Page Start Address for Page Addressing Mode 

    =====================================================================================*/

    OLED_WriteCtrl(0x20);    /* ==========================================================*/
    OLED_WriteCtrl(0x10);    /* Set Memory Addressing Mode 
                               A[1:0] = 00b, Horizontal Addressing Mode
                               A[1:0] = 01b, Vertical Addressing Mode
                               A[1:0] = 10b, Page Addressing Mode (RESET)
                               A[1:0] = 11b, Invalid 
                            =============================================================*/

    OLED_WriteCtrl(0x00);    /* ==========================================================
                               Set Lower Column Start Address for Page Addressing Mode
                               Set the lower nibble of the column start address register
                               for Page Addressing Mode.
                               This command is only for page addressing mode
                            =============================================================*/

    OLED_WriteCtrl(0x10);    /* ==========================================================
                               Set Higher Column Start Address for Page Addressing Mode
                               Set the higher nibble of the column start address register
                               for Page Addressing Mode.
                               This command is only for page addressing mode
                            =============================================================*/

    OLED_WriteCtrl(0xB0);    /* ==========================================================
                               Set Page Start Address for Page Addressing Mode 
                               
                               This command is only for page addressing mode
                            =============================================================*/

    /* ==================================================================================
    
    Hardware Configuration (Panel resolution & layout related) Command
    - 0x40~0x7F:    Set Display Start Line
    - 0xA0/0xA1:    Set Segment Re-map
    - 0xA8:         Set Multiplex Ratio
    - 0xC0/0xC8:    Set COM Output Scan Direction 
    - 0xD3:         Set Display Offset
    - 0xDA:         Set COM Pins Hardware Configuration

    =====================================================================================*/

    OLED_WriteCtrl(0x40);    /* ==========================================================
                               Set Display Start Line
                               Set display RAM display start line register from 0-63
                               With value equal to 0, RAM row 0 is mapped to COM0.
                               With value equal to 1, RAM row 1 is mapped to COM0 and so on.
                            =============================================================*/

    OLED_WriteCtrl(0xA1);    /* ==========================================================
                               Set Segment Re-map
                               This command changes the mapping between the display data
                               column address and the segment driver.
                               + 0xA0: Column address 0 is mapped to SEG0.
                               + 0xA1: Column address 127 is mapped to SEG0.
                            =============================================================*/

    OLED_WriteCtrl(0xA8);    /* ==========================================================*/
    OLED_WriteCtrl(0x3F);    /* Set MUX ratio to N+1 MUX
                               This command switches the default 63 multiplex mode to any
                               multiplex ratio, ranging from 16 to 63.
                               A[5:0] from 0 to 14 are invalid entry.
                            =============================================================*/

    OLED_WriteCtrl(0xC8);    /* ==========================================================
                               Set COM Output Scan Direction 
                               This command sets the scan direction of the COM output.
                               Additionally, the display will show once this command is issued.
                               + 0xC0: normal mode (RESET) Scan from COM0 to COM[N –1].
                               + 0xC8: remapped mode. Scan from COM[N-1] to COM0.
                               Where N is the Multiplex ratio.
                            =============================================================*/

    OLED_WriteCtrl(0xD3);    /* ==========================================================*/
    OLED_WriteCtrl(0x00);    /* Set Display Offset
                               Specifies the mapping of the display start line to one of COM0~COM63.
                               Assuming that COM0 is the display start line then the display start
                               line register is equal to 0.
                               The value is reset to 00h after RESET.
                            =============================================================*/

    OLED_WriteCtrl(0xDA);    /* ==========================================================*/
    OLED_WriteCtrl(0x12);    /* Set COM Pins Hardware Configuration
                               This command sets the COM signals pin configuration to match
                               the OLED panel hardware layout.
                               + A[4]=0b, Sequential COM pin configuration
                               + A[4]=1b(RESET), Alternative COM pin configuration 
                               + A[5]=0b(RESET), Disable COM Left/Right remap
                               + A[5]=1b, Enable COM Left/Right remap 
                            =============================================================*/

    /* ==================================================================================
    
    Timing & Driving Scheme Setting Command Table
    - 0xDB:         Set VCOMH Deselect Level
    - 0xD9:         Set Pre-charge Period
    - 0xD5:         Set Display Clock Divide Ratio/Oscillator Frequency
    - 0xA6/0xA7:    Set Normal/Inverse Display
    - 0xAE/0xAF:    Set Display ON/OFF

    =====================================================================================*/
    
    OLED_WriteCtrl(0xDB);    /* ==========================================================*/
    OLED_WriteCtrl(0x20);    /* VCOMH: The pin for COM signal deselected voltage level.
                               Set VCOMH Deselect Level
                               A[6-4]           VCOMH deselect level
                               000b             ~ 0.65 x VCC
                               001b             ~ 0.77 x VCC (RESET)
                               011b             ~ 0.83 x VCC
                            =============================================================*/

    OLED_WriteCtrl(0xD9);    /* ==========================================================*/
    OLED_WriteCtrl(0x22);    /* Set Pre-charge Period
                               This command is used to set the duration of the pre-charge period.
                               The interval is counted in number of DCLK, where RESET equals 2 DCLKs.
                            =============================================================*/
    
    OLED_WriteCtrl(0xD5);    /* ==========================================================*/
    OLED_WriteCtrl(0xF0);    /* Display Clock Divide Ratio/Oscillator Frequency
                            =============================================================*/

    OLED_WriteCtrl(0x8D);    /* ==========================================================*/
    OLED_WriteCtrl(0x14);    /* Enable charge pump regulator
	                        =============================================================*/

    OLED_WriteCtrl(0xAF);    /* Turn on OLED panel display */
    #endif
}

/* ======================================================================================

    <!> Continuous Horizontal Scroll Setup
    
    This command consists of consecutive bytes to set up the horizontal scroll parameters
    and determines the scrolling start page, end page and scrolling speed.
    Before issuing this command the horizontal scroll must be deactivated (2Eh).
    Otherwise, RAM content may be corrupted.

    Hex     D7  D6  D5  D4  D3  D2  D1  D0
    26/27   0   0   1   0   0   1   1   Xo      26h, X[0]=0, Right Horizontal Scroll
                                                27h, X[0]=1, Left Horizontal Scroll
                                                (Horizontal scroll by 1 column)
    A[7:0]  0   0   0   0   0   0   0   0       A[7:0] : Dummy byte (Set as 00h)
    B[2:0]  *   *   *   *   *   B2  B1  B0      B[2:0] : Define start page address 
                                                000b - PAGE0
                                                001b - PAGE1
                                                010b - PAGE2
                                                011b - PAGE3
                                                100b - PAGE4
                                                101b - PAGE5
                                                110b - PAGE6
                                                111b - PAGE7
    C[2:0]  *   *   *   *   *   C2  C1  C0      Set time interval between each scroll step in
                                                terms of frame frequency.
                                                000b - 5 frames
                                                001b - 64 frames
                                                010b - 128 frames
                                                011b - 256 frames
                                                100b - 3 frames
                                                101b - 4 frames
                                                110b - 25 frames
                                                111b - 2 frames
    D[2:0]  *   *   *   *   *   D2  D1  D0      Define end page address
                                                000b - PAGE0
                                                001b - PAGE1
                                                010b - PAGE2
                                                011b - PAGE3
                                                100b - PAGE4
                                                101b - PAGE5
                                                110b - PAGE6
                                                111b - PAGE7
    E[7:0]  0   0   0   0   0   0   0   0       E[7:0] : Dummy byte (Set as 00h) 
    F[7:0]  1   1   1   1   1   1   1   1       F[7:0] : Dummy byte (Set as FFh)

    <!> Continuous Horizontal Scroll Setup
    
    This command consists of 6 consecutive bytes to set up the continuous vertical scroll parameters and
    determines the scrolling start page, end page, scrolling speed and vertical scrolling offset. 
    Before issuing this command the horizontal scroll must be deactivated (2Eh).
    Otherwise, RAM content may be corrupted.

    If the vertical scrolling offset byte E[5:0] is set to zero,
    then only horizontal scrolling is performed (like command 26/27h). 

    Hex     D7  D6  D5  D4  D3  D2  D1  D0
    29/2A   0   0   1   0   0   1   X1   Xo     29h, X1X0=01b, Vertical and Right Horizontal Scroll
                                                2Ah, X1X0=10b, Vertical and Left Horizontal Scroll
                                                (Horizontal scroll by 1 column)
    A[7:0]  0   0   0   0   0   0   0   0       A[7:0] : Dummy byte (Set as 00h)
    B[2:0]  *   *   *   *   *   B2  B1  B0      B[2:0] : Define start page address 
                                                000b - PAGE0
                                                001b - PAGE1
                                                010b - PAGE2
                                                011b - PAGE3
                                                100b - PAGE4
                                                101b - PAGE5
                                                110b - PAGE6
                                                111b - PAGE7
    C[2:0]  *   *   *   *   *   C2  C1  C0      Set time interval between each scroll step in
                                                terms of frame frequency.
                                                000b - 5 frames
                                                001b - 64 frames
                                                010b - 128 frames
                                                011b - 256 frames
                                                100b - 3 frames
                                                101b - 4 frames
                                                110b - 25 frames
                                                111b - 2 frames
    D[2:0]  *   *   *   *   *   D2  D1  D0      Define end page address
                                                000b - PAGE0
                                                001b - PAGE1
                                                010b - PAGE2
                                                011b - PAGE3
                                                100b - PAGE4
                                                101b - PAGE5
                                                110b - PAGE6
                                                111b - PAGE7
    E[5:0]  *   *   E5  E4  E3  E2  E1  E0      E[5:0] : Vertical scrolling offset
                                                e.g. E[5:0]=01h refer to offset =1 row
                                                     E[5:0]=3Fh refer to offset =63 rows

    <!> Deactivate scroll - 2E
        Stop scrolling that is configured by command 26h/27h/29h/2Ah.
        After sending 2Eh command to deactivate the scrolling action, the ram data needs to be rewritten. 

    <!> Activate scroll - 2F
        Start scrolling that is configured by the scrolling setup commands :26h/27h/29h/2Ah
        with the following valid sequences: 
        Valid command sequence 1: 26h ;2Fh.
        Valid command sequence 2: 27h ;2Fh.
        Valid command sequence 3: 29h ;2Fh.
        Valid command sequence 4: 2Ah ;2Fh.

    <!> Set Vertical Scroll Area 
    
    This command consists of 3 consecutive bytes to set up the vertical scroll area.

    Hex     D7  D6  D5  D4  D3  D2  D1  D0
    A3      0   0   1   0   0   0   1   1       
    A[5:0]  0   0   A5  A4  A3  A2  A1  A0      A[5:0] : Set No. of rows in top fixed area.
    B[6:0]  *   B6  B5  B4  B3  B2  B1  B0      B[6:0] : Set No. of rows in scroll area.
                                                Note
                                                1)  A[5:0]+B[6:0] <= MUX ratio
                                                2)  B[6:0] <= MUX ratio
                                                3a) Vertical scrolling offset (E[5:0] in 29h/2Ah) <
                                                    B[6:0]
                                                3b) Set Display Start Line (X5X4X3X2X1X0 of
                                                    40h~7Fh) < B[6:0]
                                                4)  The last row of the scroll area shifts to the first row
                                                    of the scroll area.
                                                5)  For 64d MUX display
                                                    A[5:0] = 0, B[6:0]=64 : whole area scrolls
                                                    A[5:0]= 0, B[6:0] < 64 : top area scrolls
                                                    A[5:0] + B[6:0] < 64 : central area scrolls
                                                    A[5:0] + B[6:0] = 64 : bottom area scrolls 
=========================================================================================*/
