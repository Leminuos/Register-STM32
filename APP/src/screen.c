#include "screen.h"
#include "FreeRTOS.h"
#include "portable.h"

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

static SnakePixel Snake;
static int16_t tar_hori;
static int16_t tar_vert;
static int16_t new_hori = 0;
static int16_t new_vert = 0;
static uint16_t LengthSnake;
static SnakeChain* HeadSnake;
static SnakeChain* TailSnake;

static void CreatePixel(uint8_t hor, uint8_t ver)
{
    void* pixel = pvPortMalloc(sizeof(SnakeChain));

    if (TailSnake == NULL)
    {
        TailSnake = (SnakeChain*) pixel;
        TailSnake->prev = NULL;
        TailSnake->axes.horizontal = hor;
        TailSnake->axes.vertical = ver;
        HeadSnake = TailSnake;
    }
    else
    {
        TailSnake->prev = pixel;
        TailSnake = (SnakeChain*)pixel;
        TailSnake->axes.horizontal = hor;
        TailSnake->axes.vertical = ver;
        TailSnake->prev = NULL;
    }

    ST7735_DrawSquare(
        TailSnake->axes.horizontal,
        TailSnake->axes.vertical,
        Snake.size,
        Snake.front.color
    );

    ++LengthSnake;
}

static void RemovePixel()
{
    SnakeChain* temp = NULL;

    temp = HeadSnake;
    ST7735_DrawSquare(  
        temp->axes.horizontal,
        temp->axes.vertical,
        Snake.size,
        Snake.back.color
    );
    HeadSnake = HeadSnake->prev;
    vPortFree(temp);
    --LengthSnake;
}

static uint8_t CheckSnake(uint8_t hor, uint8_t ver)
{
    SnakeChain* temp = NULL;

    temp = HeadSnake;
    
    if (hor > ST7735_MAX_HORIZONTAL)
        return STATUS_COLLISION_SNAKE;
    
    if (ver > ST7735_MAX_VERTICAL)
        return STATUS_COLLISION_SNAKE;

    while (temp != NULL)
    {
        if (temp->axes.horizontal == hor && temp->axes.vertical == ver)
            return STATUS_COLLISION_SNAKE;

        temp = temp->prev;
    }

    return STATUS_NO_COLLISION_SNAKE;
}

// https://github.com/SiliconLabs/platform_applications/blob/master/platform_adc_rng/src/main.c
// https://siliconlabs.my.site.com/community/s/article/generating-an-8-bit-random-number-using-adc?language=en_US

extern uint16_t ADC_ReadData(void);

static uint8_t RamdomNumber(void)
{
    uint8_t i = 0;
    uint8_t ramdom = 0;
    uint8_t sample = 0;

    for (i = 0; i < 3; ++i)
    {
        sample = ADC_ReadData() & 0x07;
        ramdom |= sample << (i << 3);
    }
    
    return ramdom;
}

static void GenerateTarget(void)
{
    do {
        tar_hori = (RamdomNumber() % ((ST7735_MAX_HORIZONTAL / Snake.size) + 1)) * Snake.size;
        tar_vert = (RamdomNumber() % ((ST7735_MAX_VERTICAL / Snake.size) + 1)) * Snake.size;
    } while (CheckSnake(tar_hori, tar_vert) == STATUS_COLLISION_SNAKE);

    ST7735_DrawSquare(  
        tar_hori,
        tar_vert,
        Snake.size,
        Snake.target.color
    );
}

static void RemoveTarget(void)
{
    ST7735_DrawSquare(  
        tar_hori,
        tar_vert,
        Snake.size,
        Snake.back.color
    );
}

void SnakeUpdateDirection(uint8_t dir)
{
    // if (Snake.dir != 0xFF && (Snake.dir + 2) % 4 == dir)
    //     return;
    
    Snake.old_dir = Snake.dir;
    Snake.dir = dir;
}

uint8_t GetStateSnake()
{
    return Snake.state;
}

void SetStateSnake(uint8_t state)
{
    Snake.state = state;
}

void SnakeRun(void)
{
    new_hori = TailSnake->axes.horizontal;
    new_vert = TailSnake->axes.vertical;

    switch (Snake.dir)
    {
        case BUTTON_KEY_RIGHT:
        new_vert = new_vert + Snake.size;

        if (new_vert >= ST7735_MAX_VERTICAL)
        {
            new_vert = new_vert - ST7735_MAX_VERTICAL;
        }
        break;

        case BUTTON_KEY_LEFT:
        new_vert = new_vert - Snake.size;

        if (new_vert < 0)
        {
            new_vert = new_vert + ST7735_MAX_VERTICAL;
        }
        break;

        case BUTTON_KEY_DOWN:
        new_hori = new_hori - Snake.size;

        if (new_hori < 0)
        {
            new_hori = new_hori + ST7735_MAX_HORIZONTAL;
        }
        break;

        case BUTTON_KEY_UP:
        new_hori = new_hori + Snake.size;

        if (new_hori >= ST7735_MAX_HORIZONTAL)
        {
            new_hori = new_hori - ST7735_MAX_HORIZONTAL;
        }
        break;

        default:
        return;
    }

    if (CheckSnake(new_hori, new_vert) == STATUS_COLLISION_SNAKE)
    {
        Snake.state = SNAKE_STOP_STATE;
        return;
    }

    CreatePixel(new_hori, new_vert);

    if (new_hori == tar_hori && new_vert == tar_vert)
    {
        GenerateTarget();
        return;
    }

    RemovePixel();
}

uint32_t SnakeOperation(void)
{
    uint32_t wait = 0;

    switch (Snake.state)
    {
        case SNAKE_START_STATE:
        Snake.dir = 0xFF;
        CreatePixel(new_hori, new_vert);
        GenerateTarget();
        wait = 0xFFFFFFFF;
        break;

        case SNAKE_RUN_STATE:
        SnakeRun();
        wait = 250;
        break;
        
        case SNAKE_STOP_STATE:
        RemoveTarget();
        while (HeadSnake != NULL) RemovePixel();
        TailSnake = 0;
        Snake.state = SNAKE_START_STATE;
        wait = 0;
        break;

        default:
        break;
    }
    
    return wait;
}

void SnakeInit(SnakePixel* Inf)
{
    Snake.axes.horizontal = Inf->axes.horizontal;
    Snake.axes.vertical   = Inf->axes.vertical;
    Snake.size            = Inf->size;
    Snake.state           = SNAKE_START_STATE;
    Snake.back            = Inf->back;
    Snake.front           = Inf->front;
    Snake.dir             = Inf->dir;
    Snake.old_dir         = 0;
    Snake.target          = Inf->target;
}
