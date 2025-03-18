#ifndef __SPI_FLASH_CONFIG__
#define __SPI_FLASH_CONFIG__

/* 0x05 - Read Unique ID Number
 * 0x04 - Read JEDEC ID
 * 0x03 - Release Power-down / Device ID
 * 0x02 - Read Manufacturer / Device ID Quad I/O
 * 0x01 - Read Manufacturer / Device ID Dual I/O
 * 0x00 - Read Manufacturer / Device ID
 * */
#define STANDARD_SPI            1
#define QUAD_SPI                0
#define DUAL_SPI                0
#define MODE_DEVICE_ID          0x05
#define SPI_FLASH_INSTANCE      SPI2
#define FAST_READ               0
#define DEVICE_ID               0xD262BC629B390F2A

#endif  /* __SPI_FLASH_CONFIG__ */

