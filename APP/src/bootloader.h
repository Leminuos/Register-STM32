#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include <stm32f103.h>

/* BLDC - Bootloader Device Controller */
#define BOOT_HEADER                 0x424C4443
#define PROGRAM_START_ADDRESS       0x20000000
#define FLASH_START_ADDRESS         0x08000000
#define FLASH_MAX_SIZE              0x10000
#define RAM_START_ADDRESS           0x20000000
#define RAM_SIZE_MAX                0x5000
#define MAX_BOOT_BUFFER_SIZE        48
#define MAX_BOOT_REQUEST_SIZE       (MAX_BOOT_BUFFER_SIZE + 14)
#define MAX_BOOT_RESPONSE_SIZE      (MAX_BOOT_BUFFER_SIZE + 10)
#define MAX_BOOT_CRC_SIZE           4

/* Request command */
#define BOOT_REQ_CMD_ERASE          0x00
#define BOOT_REQ_CMD_READ           0x01
#define BOOT_REQ_CMD_WRITE          0x02
#define BOOT_REG_CMD_ADDRESS        0x03
#define BOOT_REQ_CMD_RESET          0xFE
#define BOOT_REQ_CMD_VERSION        0xFF

/* Respond status */
#define BOOT_RES_NACK               0x00
#define BOOT_RES_ACK                0x01
#define BOOT_RES_VALID              0x02

typedef enum {
    BOOT_SUCCESS = 0,
    INVALID_HEADER_ERR = -1,
    INVALID_LENGTH_ERR = -2,
    INVALID_DATA_ERR = -3,
} boot_state_t;

typedef struct __attribute__((packed)) {
    uint32_t    header;
    uint8_t     command;
    uint32_t    address;
    uint8_t     length;
    uint8_t     data[MAX_BOOT_BUFFER_SIZE];
    uint32_t    crc;
}  boot_packet_req_t;

typedef struct __attribute__((packed)) {
    uint32_t    header;
    uint8_t     status;
    uint8_t     length;
    uint8_t     data[MAX_BOOT_BUFFER_SIZE];
    uint32_t    crc;
}  boot_packet_res_t;

extern void BootloaderInit(void);
extern void UpdateFirmware(void);
extern void JumpToApplication(void);
extern void BootloaderPolling(void);

#endif /* __BOOTLOADER_H__ */

