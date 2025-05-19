#include "stm32_driver_flash.h"

static void Flash_Unlock(void)
{
    if (FLASH_CR & CTL_LOCK)
    {
        // Unlock the flash memory
        FLASH_KEYR = 0x45670123;
        FLASH_KEYR = 0xCDEF89AB;
        FLASH_CR &= ~CTL_LOCK; // Clear the lock bit
    }
}

static void Flash_Lock(void)
{
    // Lock the flash memory
    FLASH_CR |= CTL_LOCK;
}

void FLash_PageErase(uint8_t page)
{
    uint32_t address = 0;

    address = page * FLASH_PAGE_SIZE + FLASH_BASE_ADDR;

    while (FLASH_SR & STATUS_BSY);  // Wait for the flash to be ready
    Flash_Unlock();                 // Unlock the flash memory
    FLASH_CR |= CTR_PAGE_ERASE;     // Set the page erase bit
    FLASH_AR = address;             // Set the address to erase
    FLASH_CR |= CTL_START;          // Start the erase operation
    while (FLASH_SR & STATUS_BSY);  // Wait for the flash to be ready
    FLASH_CR &= ~CTR_PAGE_ERASE;    // Clear the page erase bit
    Flash_Lock();                   // Lock the flash memory
}

void Flash_WriteProgram(uint32_t address, uint32_t size, uint8_t* data)
{
    uint8_t i = 0;
    uint16_t* data16 = (uint16_t*)data;
    volatile uint16_t* addr16 = (volatile uint16_t*)address;

    while (FLASH_SR & STATUS_BSY);  // Wait for the flash to be ready
    Flash_Unlock();                 // Unlock the flash memory
    FLASH_CR |= CTR_PROG;           // Set the program bit

    for (i = 0; i < size; i = i + 2)
    {
        *addr16++ = *data16++; // Write the data to the flash memory
        while (FLASH_SR & STATUS_BSY);                       // Wait for the flash to be ready
    }

    FLASH_CR &= ~CTR_PROG;          // Clear the program bit
    Flash_Lock();                   // Lock the flash memory
}

void Flash_ReadProgram(uint32_t address, uint32_t size, uint8_t* data)
{
    uint8_t i = 0;
    uint16_t* data16 = (uint16_t*)data;
    volatile uint16_t* addr16 = (volatile uint16_t*)address;
    
    for (i = 0; i < size; i = i + 2)
    {
        *data16++ = *addr16++; // Read the data from the flash memory
    }
}

