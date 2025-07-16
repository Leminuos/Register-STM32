#include "bootloader.h"
#include "string.h"
#include "stm32_driver_flash.h"
#include "stm32_driver_tim.h"
#include "stm32_driver_spi.h"
#include "stm32_hal_usb.h"
#include "spiflash.h"
#include "debug.h"

extern void delay(uint16_t mDelay);
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

void RAM_WriteProgram(uint32_t address, uint32_t size, uint8_t* src)
{
    uint32_t i = 0;
    volatile uint8_t* dst = (volatile uint8_t*) address;

    for (i = 0; i < size; ++i) *dst++ = *src++;
}

void RAM_ReadProgram(uint32_t address, uint32_t size, uint8_t* dst)
{
    uint32_t i = 0;
    volatile uint8_t* src = (volatile uint8_t*) address;

    for (i = 0; i < size; ++i) *dst++ = *src++;
}

static boot_state_t HandleHidRequest(uint8_t* data)
{
    uint32_t            i = 0;
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
        W25QXX_EraseSector(req->address);
        res.status = BOOT_RES_ACK;
        res.length = 0;

        break;

    case BOOT_REQ_CMD_READ:
        for (i = 0; i < MAX_BOOT_BUFFER_SIZE; ++i) W25QXX_ReadByte(req->address + i, &res.data[i]);
        res.status = BOOT_RES_ACK;
        res.length = MAX_BOOT_BUFFER_SIZE;

        break;

    case BOOT_REQ_CMD_WRITE:
        for (i = 0; i < req->length; ++i) W25QXX_WriteByte(req->address + i, req->data[i]);
        res.status = BOOT_RES_ACK;
        res.length = 0;
        
        break;

    case BOOT_REG_CMD_ADDRESS:
    {
        uint8_t data[4];

        data[0] = req->address & 0xFF;
        data[1] = (req->address >> 8) & 0xFF;
        data[2] = (req->address >> 16) & 0xFF;
        data[3] = (req->address >> 24) & 0xFF;

        for (i = 0; i < 4; ++i) W25QXX_WriteByte(i, data[i]);

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

void UpdateFirmware(void)
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
            state = HandleHidRequest(data);

            if (state == BOOT_SUCCESS)
            {
                // Send response back to host
                HID_SendReport(data);
            }
        }
    }
}

void LoadFirmware(void)
{
    uint8_t data = 0;
    uint32_t i = 0;
    uint32_t length = 0;

    for (i = 0; i < 4; ++i)
    {
        W25QXX_ReadByte(i, &data);
        length = length | (data << 8*i);
    }

    DEBUG(LOG_INFO, "Boot", "Length firmware: %d", length);

    for (i = 0; i < length; ++i)
    {
        W25QXX_ReadByte(0x1000 + i, &data);
        RAM_WriteProgram(RAM_START_ADDRESS + i, 1, &data);
    }
}

void JumpToApplication(void)
{
    DEBUG(LOG_VERBOSE, "Boot", "Jump to address -> 0x%08X", PROGRAM_START_ADDRESS);

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
    
    __enable_irq();
    
    reset_handle();
}

void BootloaderPolling(void)
{
    if (GPIOA->IDR.BITS.IDR0 == 0)
    {
        delay(20);

        if (GPIOA->IDR.BITS.IDR0 == 0)
        {
            DEBUG(LOG_INFO, "Boot", "Update firmware");
            UpdateFirmware();
        }
    }
    else
    {
        delay(20);

        if (GPIOA->IDR.BITS.IDR0 == 1)
        {
            LoadFirmware();

            // Jump to application
            JumpToApplication();
        }
    }
}

void BootloaderInit(void)
{
    uint8_t id[2];

    // Initialize USB
    USB_PowerOnReset();

    // Init timer
    TimerConfig();

    SPI_Init(SPI2);

    W25QXX_GetDeviceID(id);

    DEBUG(LOG_INFO, "Boot", "Flash ID: 0x%02X%02X", id[0], id[1]);
    
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
