#ifndef __DRIVER_FLASH_H
#define __DRIVER_FLASH_H

#include "stm32f103.h"

#define FLASH_PAGE_SIZE   1024
#define FLASH_BASE_ADDR   0x08000000

#define FLASH_ACR                       (*(volatile uint32_t*)0x40022000)
#define FLASH_KEYR                      (*(volatile uint32_t*)0x40022004)
#define FLASH_OPTKEYR                   (*(volatile uint32_t*)0x40022008)
#define FLASH_SR                        (*(volatile uint32_t*)0x4002200C)
#define FLASH_CR                        (*(volatile uint32_t*)0x40022010)
#define FLASH_AR                        (*(volatile uint32_t*)0x40022014)
#define FLASH_OBR                       (*(volatile uint32_t*)0x4002201C)
#define FLASH_WRPR                      (*(volatile uint32_t*)0x40022020)

/* Flash access control register */
#define PREFETCH_STATUS_ENABLE          0x20
#define PREFETCH_STATUS_DISABLE         0x00
#define PREFETCH_ENABLE                 0x10
#define PREFETCH_DISABLE                0x00
#define HALF_CYCLE_ACCESS_ENABLE        0x08
#define HALF_CYCLE_ACCESS_DISABLE       0x00
#define LATENCY_0                       0x00
#define LATENCY_1                       0x01
#define LATENCY_2                       0x02

/* Flash status register */
#define STATUS_EOP                      0x20
#define STATUS_WRITE_PROTECT_ERR        0x10
#define STATUS_PROGERR                  0x04
#define STATUS_BSY                      0x01

/* Flash control register */
#define CTL_EOP_INT_ENABLE              0x0800
#define CTL_ERR_INT_ENABLE              0x0400
#define CTL_OPT_BYTE_WRITE_ENABLE       0x0200
#define CTL_LOCK                        0x0080
#define CTL_START                       0x0040
#define CTR_OPT_BYTE_ERASE              0x0020
#define CTR_OPT_BYTE_PROG               0x0010
#define CTR_MASS_ERASE                  0x0004
#define CTR_PAGE_ERASE                  0x0002
#define CTR_PROG                        0x0001

void FLash_PageErase(uint8_t page);
void Flash_WriteProgram(uint32_t address, uint32_t size, uint8_t* data);
void Flash_ReadProgram(uint32_t address, uint32_t size, uint8_t* data);

#endif /* __DRIVER_FLASH_H */

