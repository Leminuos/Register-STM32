#include "main.h"
#include "gfx_obj.h"
#include "gfx_log.h"
#include "gfx_vdb.h"
#include "gfx_task.h"
#include "gfx_button.h"
#include "gfx_label.h"
#include "lcd_generic.h"

extern void TraceInit(void);

void gfx_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const gfx_color_t* buf)
{
    lcd_draw_bitmap(x1, y1, x2, y2, buf);
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

void lcd_delay(uint32_t ms)
{
    delay(ms);
}

void lcd_spi_init(void)
{
    SPI_Init(SPI2);
}

void lcd_spi_release_bus(void)
{
    GPIOB->ODR.BITS.ODR12 = 1;
}

void lcd_spi_require_bus(void)
{
    GPIOB->ODR.BITS.ODR12 = 0;
}

void lcd_spi_send_data(unsigned char data)
{
    SPI_WriteByte(SPI2, data);
}

uint8_t lcd_spi_recv_data(void)
{
    return SPI_ReadByte(SPI2);
}

void lcd_io_pin_reset(lcd_io_pin_t pin)
{
    switch(pin)
    {
        case LCD_DC_IO_PIN:
            GPIO_ResetBit(GPIOA, GPIO_PIN_9);
            break;

        case LCD_LED_IO_PIN:
            GPIO_ResetBit(GPIOA, GPIO_PIN_10);
            break;

        case LCD_RESET_IO_PIN:
            GPIO_ResetBit(GPIOA, GPIO_PIN_11);
            break;

        default:
            break;
    }
}

void lcd_io_pin_set(lcd_io_pin_t pin)
{
    switch(pin)
    {
        case LCD_DC_IO_PIN:
            GPIO_SetBit(GPIOA, GPIO_PIN_9);
            break;

        case LCD_LED_IO_PIN:
            GPIO_SetBit(GPIOA, GPIO_PIN_10);
            break;

        case LCD_RESET_IO_PIN:
            GPIO_SetBit(GPIOA, GPIO_PIN_11);
            break;

        default:
            break;
    }
}


void lcd_io_pin_init(lcd_io_pin_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    switch(pin)
    {
        case LCD_DC_IO_PIN:
            GPIO_InitStruct.GPIO_Pin = GPIO_PIN_9;
            break;

        case LCD_LED_IO_PIN:
            GPIO_InitStruct.GPIO_Pin = GPIO_PIN_10;
            break;

        case LCD_RESET_IO_PIN:
            GPIO_InitStruct.GPIO_Pin = GPIO_PIN_11;
            break;

        default:
            break;
    }

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    TestLed();
    lcd_drv_t drv_fn = {
        .delay = lcd_delay,
        .spi_init = lcd_spi_init,
        .spi_require_bus = lcd_spi_require_bus,
        .spi_release_bus = lcd_spi_release_bus,
        .spi_send_data = lcd_spi_send_data,
        .spi_recv_data = lcd_spi_recv_data,
        .io_pin_init = lcd_io_pin_init,
        .io_pin_reset = lcd_io_pin_reset,
        .io_pin_set = lcd_io_pin_set,
    };

    lcd_init(&drv_fn);

    gfx_init();
    gfx_obj_t* btn_handler = gfx_create_button(100, 100);
    gfx_btn_set_border(btn_handler, 5, GFX_BORDER_FULL);
    gfx_obj_align(btn_handler, GFX_ALIGN_CENTER, 0, 0);

    gfx_obj_t* txt_handler = gfx_create_label();
    gfx_label_set_text(txt_handler, "Nguyen Bui dep trai vai ca l, Nguyen Bui dep trai vai ca l");
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
