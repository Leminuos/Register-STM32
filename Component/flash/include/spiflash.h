#ifndef __SPI_FLASH__
#define __SPI_FLASH__

#include "stm32_driver_spi.h"
#include "spiflashconfig.h"
#include <stdint.h>

#define PAGE_SIZE               (256u)
#define SECTOR_SIZE             ((16U) * PAGE_SIZE)
#define BLOCK_SIZE_32KB         ((128U) * PAGE_SIZE)
#define BLOCK_SIZE_64KB         ((256U) * PAGE_SIZE)
#define CHIP_SIZE               ((65536U) * PAGE_SIZE)

#define STS_REG_1               (0x00)
#define STS_REG_2               (0x01)
#define STS_REG_3               (0x02)

#define STS_VOL                 (0x00)
#define STS_NON_VOl             (0x01)

typedef enum {
    FLASH_SUCCESS   = 0x00,
    FLASH_BUSY      = 0x01,
    FLASH_WEL       = 0x02,
    FLASH_WPS       = 0x04,
    FLASH_SUS       = 0x80,
    FLASH_TIMEOUT   = 0xFF
} FLASH_RET_STS;

typedef union {
    __IO uint8_t word[3];

    struct {                            /* Datasheet W25Q128JV */
        __I  uint8_t busy       : 1;    /* Erase/Write In Progress - Status Only
                                         * Cho biết chip đang thực hiện lệnh nào đó.
                                         * Ngoại trừ, Read Status và Erase/Program Suspend.
                                         * */
        __I  uint8_t wel        : 1;    /* Write Enable Latch - Status Only
                                         * Set lên 1 khi thực hiện lệnh Write Enable.
                                         * Clear về 0 khi write disabled.
                                         * Chip sẽ tự động clear bit này khi power on hoặc thực hiện xong các lệnh đọc ghi.
                                         * */
        __IO uint8_t bp0        : 1;    /* Block Protect Bits */
        __IO uint8_t bp1        : 1;    /* Block Protect Bits */
        __IO uint8_t bp2        : 1;    /* Block Protect Bits */
        __IO uint8_t tb         : 1;    /* Top/Bottom Block Protect */
        __IO uint8_t sec        : 1;    /* Sector/Block Protect Bit */
        __IO uint8_t srp        : 1;    // Status Register Protect
        __IO uint8_t srl        : 1;    // 2 bit SRP và SRL được sử dụng để protect các thanh ghi status register.
        __IO uint8_t qe         : 1;    /* Quad Enable
                                         * Bit này nhằm enable chế độ Quad SPI mode.
                                         * */
             uint8_t            : 1;    
        __IO uint8_t lb1        : 1;    // Security Register Lock Bits
        __IO uint8_t lb2        : 1;    // là các bit One Time Program - OTP
        __IO uint8_t lb3        : 1;    // write protect control và read status thanh ghi Security Registers
                                        // Khi set lên 1 thì thanh ghi Security Register tương ứng sẽ vĩnh viễn
                                        // ở chế độ read only.
        __IO uint8_t cmp        : 1;    /* Complement Protect
                                         * Set lên 1 sẽ đảo ngược các bit SEC, TB, BP0, BP1 và BP2.
                                         * */
        __I  uint8_t sus        : 1;    /* Erase/Program Suspend Status
                                         * Set hoặc clear khi thực hiện lệnh Erase/Program Suspend (75h)
                                         * */
             uint8_t            : 2;    
        __IO uint8_t wps        : 1;    /* Write Protect Selection
                                         * WPS = 0 thì chip sẽ kết hợp các bit CMP, SEC, TB, BP[2: 0]
                                         * để protect một vùng nhớ cụ thể.
                                         * WPS = 1 thì chip sẽ sử dụng Individual Block Locks để protect
                                         * bất kỳ sector hoặc block riêng lẻ.
                                         * */ 
             uint8_t            : 2;
        __IO uint8_t drv0       : 1;    // Output Driver Strength
        __IO uint8_t drv1       : 1;    // Xác định output strength cho hoạt động đọc.
        union {
            __IO uint8_t hold   : 1;
            __IO uint8_t reset  : 1;
        };
    } bits;
} W25QXX_Status;

void W25QXX_PowerDown(void);
void W25QXX_EraseResume(void);
void W25QXX_ResetDevice(void);
void W25QXX_EraseSuspend(void);
void W25QXX_WriteDisable(void);
void W25QXX_ReleasePowerDown(void);
void W25QXX_WriteEnable(uint8_t vol);
FLASH_RET_STS W25QXX_ChipErase(void);
FLASH_RET_STS W25QXX_ChipErase(void);
void W25QXX_GetDeviceID(uint8_t* ret);
uint8_t W25QXX_ReadStatus(uint8_t res);
FLASH_RET_STS W25QXX_GlobalLock(void);
FLASH_RET_STS W25QXX_GlobalUnLock(void);
FLASH_RET_STS W25QXX_EraseSector(uint16_t sector);
FLASH_RET_STS W25QXX_EraseBlock32KB(uint8_t block);
FLASH_RET_STS W25QXX_EraseBlock64KB(uint8_t block);
FLASH_RET_STS W25QXX_BlockSectorLock(uint32_t addr);
FLASH_RET_STS W25QXX_BlockSectorUnLock(uint32_t addr);
FLASH_RET_STS W25QXX_WriteStatus(uint8_t res, uint8_t val);
FLASH_RET_STS W25QXX_ReadByte(uint32_t addr, uint8_t *ret);
FLASH_RET_STS W25QXX_WriteByte(uint32_t addr, uint8_t data);
FLASH_RET_STS W25QXX_ReadBlockSectorLock(uint32_t addr, uint8_t* ret);
FLASH_RET_STS W25QXX_WriteData(uint32_t addr, uint8_t* buff, uint32_t len);
FLASH_RET_STS W25QXX_ReadData(uint32_t addr, uint8_t* ret, uint32_t size);

#endif  /* __SPI_FLASH__ */
