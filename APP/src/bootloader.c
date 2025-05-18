#include "bootloader.h"
#include "string.h"
#include "stm32_driver_flash.h"
#include "stm32_driver_tim.h"
#include "stm32_hal_usb.h"

extern void delay(uint32_t mDelay);
typedef void (*app_entry_t)(void);

static uint32_t CalculateCRC(uint8_t* data, uint32_t length)
{
    uint32_t i, j;
    uint32_t crc = 0xFFFFFFFF;

    for (i = 0; i < length; i++)
    {
        crc = crc ^ data[i];

        for (j = 0; j < 32; j++)
        {
            if (crc & 0x80000000)
            {
                crc = (crc << 1) ^ 0x04C11DB7;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

void BootloaderInit(void)
{
    // Initialize USB
    USB_PowerOnReset();

    // Init timer
    TimerConfig();
    
    // Initialize GPIO for bootloader mode
    RCC->APB2ENR.BITS.IOPCEN = 0x01;
    GPIOC->CRH.BITS.MODE13 = 0x03; // Output mode, max speed 50 MHz
    GPIOC->CRH.BITS.CNF13 = 0x00;  // General purpose output push-pull
    GPIOC->ODR.BITS.ODR13 = 0;     // Set PC13 low
    delay(20);
    GPIOC->ODR.BITS.ODR13 = 1;     // Set PC13 high

    RCC->APB2ENR.BITS.IOPAEN = 0x01;
    GPIOA->CRL.BITS.MODE0 = 0x00; // Input mode
    GPIOA->CRL.BITS.CNF0 = 0x02;  // General purpose input push-pull
    GPIOA->ODR.BITS.ODR0 = 1;     // Input pull-up
}

boot_state_t BootloaderHandle(uint8_t* data)
{
    uint32_t            crc = 0;
    boot_packet_res_t   res = {0};
    boot_packet_req_t*  req = NULL;
    boot_state_t        state = BOOT_SUCCESS;

    req = (boot_packet_req_t*)data;
    memset(&res, 0, sizeof(res));
    res.header = req->header;

    // Check header
    if (req->header != BOOT_HEADER)
    {
        res.status = BOOT_RES_NACK;
        res.length = 0;
        state = INVALID_HEADER_ERR;
        goto end;
    }

    // Check length
    if (req->length > MAX_BOOT_BUFFER_SIZE)
    {
        res.status = BOOT_RES_NACK;
        res.length = 0;
        state = INVALID_LENGTH_ERR;
        goto end;
    }

    // Check CRC
    crc = CalculateCRC(data, MAX_BOOT_REQUEST_SIZE - MAX_BOOT_CRC_SIZE);
    if (crc != req->crc)
    {
        res.status = BOOT_RES_NACK;
        res.length = 0;
        state = INVALID_DATA_ERR;
        goto end;
    }

    switch (req->command)
    {    
    case BOOT_REQ_CMD_ERASE:
        FLash_PageErase((req->address & 0x0000FFFF) / FLASH_PAGE_SIZE);
        res.status = BOOT_RES_ACK;
        res.length = 0;
        break;

    case BOOT_REQ_CMD_READ:
        Flash_ReadProgram(req->address, MAX_BOOT_BUFFER_SIZE, res.data);
        res.status = BOOT_RES_ACK;
        res.length = MAX_BOOT_BUFFER_SIZE;
        break;

    case BOOT_REQ_CMD_WRITE:
    {
        uint8_t i = 0;
        Flash_ReadProgram(req->address, req->length, res.data);

        for (i = 0; i < req->length; ++i)
        {
            if (res.data[i] != 0xFF)
            {
                res.status = BOOT_RES_NACK;
                res.length = 0;
                state = INVALID_DATA_ERR;
                goto end;
            }
        }

        Flash_WriteProgram(req->address, req->length, req->data);
        res.status = BOOT_RES_ACK;
        res.length = 0;
        break;
    }

    case BOOT_REQ_CMD_RESET:
        NVIC_SystemReset();
        break;
    
    case BOOT_REQ_CMD_VERSION:
        break;
    
    default:
        break;
    }

end:
    memset(data, 0, HID_MAX_SIZE_REPORT);
    memcpy(data, &res, sizeof(res));
    return state;
}

void BootloaderProcess(void)
{
    uint8_t data[HID_MAX_SIZE_REPORT];
    boot_state_t state = BOOT_SUCCESS;

    // Enable timer
    TIM2->DIER.BITS.UIE = 0x01;
    TIM2->CR1.BITS.CEN = 0x01;

    // Process bootloader commands
    while (1)
    {
        // Receive data from USB
        if (HID_ReceiveReport(data) > 0)
        {
            state = BootloaderHandle(data);

            if (state == BOOT_SUCCESS)
            {
                // Send response back to host
                HID_SendReport(data);
            }
        }
    }
}

void JumpToApplication(void)
{
    /* Disable all interrupt */
    __disable_irq();

    /* Turn off System tick */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;

    /* Clear Pending Interrupt Request*/
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

    /* Set main stack pointer */
    __set_MSP(*((volatile uint32_t*) PROGRAM_START_ADDRESS));

    __DMB();
    SCB->VTOR = PROGRAM_START_ADDRESS;
    __DSB();

    /* Jump to application */
    app_entry_t reset_handle = (app_entry_t)(*(volatile uint32_t*) (PROGRAM_START_ADDRESS + 4));
    reset_handle();
}
