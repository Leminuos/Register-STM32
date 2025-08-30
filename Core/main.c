#include "main.h"
#include "gfx_obj.h"
#include "gfx_log.h"
#include "gfx_vdb.h"
#include "gfx_task.h"
#include "gfx_button.h"
#include "ili9341.h"

extern void TraceInit(void);

extern void ILI9341_SendData(uint8_t data);

void gfx_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const gfx_color_t* buf)
{
    uint32_t i, j;
    gfx_color_int_t pixel = 0;
    gfx_color_int_t color = 0;

    ILI9341_CS_LOW;
    ILI9341_SetWindow(x1, y1, x2, y2);

    for (i = x1; i <= x2; i++) {
        for (j = y1; j <= y2; j++)
        {
            color = buf[pixel].full;
            ILI9341_SendData(color >> 8);
            ILI9341_SendData(color & 0xFF);
            ++pixel;
        }
    }

    ILI9341_CS_HIGH;

    gfx_flush_ready();
}

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    TestLed();
    ILI9341_Init();
    gfx_init();
    gfx_obj_t* btn_handler = gfx_create_button(50, 100);
    gfx_btn_set_border(btn_handler, GFX_COLOR_BLACK, 4, GFX_BORDER_FULL);
}

void loop(void)
{
    gfx_task_handler();
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}
