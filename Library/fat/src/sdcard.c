#include "stm32_driver_spi.h"
#include "stm32_driver_gpio.h"

#define	SD_DESELECT()		GPIOB->ODR.BITS.ODR12 = 1	/* Set MMC CS "high" */
#define SD_SELECT()		    GPIOB->ODR.BITS.ODR12 = 0	/* Set MMC CS "low" */
#define SPI_HANDLER	        SPI2
#define CMD_SIZE            6U

extern uint32_t HAL_GetTick(void);
extern void delay(uint32_t mDelay);

/*
R1: 0abcdefg
     ||||||`- 1th bit (g): card is in idle state
     |||||`-- 2th bit (f): erase sequence cleared
     ||||`--- 3th bit (e): illigal command detected
     |||`---- 4th bit (d): crc check error
     ||`----- 5th bit (c): error in the sequence of erase commands
     |`------ 6th bit (b): misaligned addres used in command
     `------- 7th bit (a): command argument outside allowed range
             (8th bit is always zero)
*/

#define SD_IDLE_STATE          0x01

static int SDCARD_ReadBytes(uint8_t* buff, size_t buff_size) {
    while(buff_size > 0) {
        *buff = SPI_ReceiveByte(SPI_HANDLER);
        buff++;
        buff_size--;
    }

    return 0;
}

static uint8_t SDCARD_ReadR1() {
    uint8_t r1 = 0;
    uint8_t i = 0;

    for(i = 0; i < 10; ++i) {
        r1 = SPI_ReceiveByte(SPI_HANDLER);
        if((r1 & 0x80) == 0) break;
    }

    return r1;
}

static void SDCARD_ReadR7(uint8_t* res) {
    uint8_t i = 0;

    for(i = 0; i < 10; ++i) {
        res[0] = SPI_ReceiveByte(SPI_HANDLER);
        if((res[0] & 0x80) == 0) break;
    }

    for (i = 1; i < 5; ++i)
        res[i] = SPI_ReceiveByte(SPI_HANDLER);
}

static int SDCARD_WaitNotBusy() {
    uint8_t busy;
    uint8_t i = 0;

    for (i = 0; i < 10; ++i) {
        SDCARD_ReadBytes(&busy, sizeof(busy));
        if (busy == 0xFF) return 0;
    }

    return -1;
}

// data token for CMD9, CMD17, CMD18 and CMD24 are the same
#define DATA_TOKEN_CMD9  0xFE
#define DATA_TOKEN_CMD17 0xFE
#define DATA_TOKEN_CMD18 0xFE
#define DATA_TOKEN_CMD24 0xFE
#define DATA_TOKEN_CMD25 0xFC

static int SDCARD_WaitDataToken(uint8_t token) {
    uint8_t fb;
    
    for(;;) {
        fb = SPI_ReceiveByte(SPI_HANDLER);
        if(fb == token)
            break;

        if(fb != 0xFF)
            return -1;
    }
    return 0;
}

int SD_GetBlockNums(uint32_t* num)
{
    uint8_t csd[16] = {0};
    uint8_t crc[2];
    uint8_t i = 0;

    SD_SELECT();

    if(SDCARD_WaitNotBusy() < 0) { // keep this!
        SD_DESELECT();
        return -1;
    }

    /* CMD9 (SEND_CSD) command */

    {
        uint8_t res = 0;
        uint8_t cmd[] = {
            0x40 | 0x09 /* CMD9 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */
        };

        for (i = 0; i < CMD_SIZE; ++i)
        {
            SPI_WriteByte(SPI_HANDLER, cmd[i]);
        }
        
        res = SDCARD_ReadR1();
        
        if (res != 0x00)
        {
            SD_DESELECT();
            return -2;
        }
    }

    if(SDCARD_WaitDataToken(DATA_TOKEN_CMD9) < 0) {
        SD_DESELECT();
        return -3;
    }

    while (1)
    {
        GPIO_SetBit(GPIOC, GPIO_PIN_13);
        delay(100);
        GPIO_ResetBit(GPIOC, GPIO_PIN_13);
        delay(100);
    }

}

void SD_ReadSingleBlock(uint32_t nBlocks, uint8_t* buff)
{
    uint8_t i = 0;
    uint8_t res = 0;
    uint8_t crc[2] = {0};

    SD_SELECT();

    if (SDCARD_WaitNotBusy() < 0)
    {
        SD_DESELECT();
        return;
    }

    uint8_t cmd[] = {
        0x40 | 0x11 /* CMD17 */,
        (nBlocks >> 24) & 0xFF,
        (nBlocks >> 16) & 0xFF,
        (nBlocks >> 8) & 0xFF,
        nBlocks & 0xFF /* ARG */,
        (0x7F << 1) | 1 /* CRC7 + end bit */ 
    };

    for (i = 0; i < 6; ++i)
    {
        SPI_WriteByte(SPI_HANDLER, cmd[i]);
    }

    res = SDCARD_ReadR1();

    if (res != 0x00)
    {
        SD_DESELECT();
        return;
    }

    if (SDCARD_WaitDataToken(DATA_TOKEN_CMD17))
    {
        SD_DESELECT();
        return;
    }

    if(SDCARD_ReadBytes(buff, 512) < 0) {
        SDCARD_Unselect();
        return -4;
    }

    if(SDCARD_ReadBytes(crc, 2) < 0) {
        SDCARD_Unselect();
        return -5;
    }

    SDCARD_Unselect();
    return 0;
}

void SD_Init()
{
    uint32_t i = 0;

    SPI_Init(SPI_HANDLER);

    delay(10);

    /*
    Step 1.

    Set DI và CS ở mức cao trong khoảng 74 xung nhịp CLK. Nếu không SD card
    sẽ không hoạt động được.
    */

    SD_DESELECT();

    for (i = 0; i < 10; ++i)
    {
        SPI_WriteByte(SPI_HANDLER, 0xFF);
    }

    SD_SELECT();

    /*
    Bước 2.

    Gửi command CMD0 (GO_IDLE_STATE) để reset SD card.
    */

    {
        if (SDCARD_WaitNotBusy() < 0)
        {
            SD_DESELECT();
            return;
        }
    
        uint8_t res = 0;
        uint8_t cmd[] = {
            0x40 | 0x00 /* CMD0 */, 0x00, 0x00, 0x00, 0x00 /* ARG = 0 */, (0x4A << 1) | 1 /* CRC7 + end bit */
        };

        for (i = 0; i < CMD_SIZE; ++i)
        {
            SPI_WriteByte(SPI_HANDLER, cmd[i]);
        }
        
        res = SDCARD_ReadR1();
        
        if (res != SD_IDLE_STATE)
        {
            SD_DESELECT();
            return;
        }
    }

    /*
    Step 3.

    Khi card vào trạng thái idle state, ta gửi command CMD8 với argument là
    0x000001AA và CRC trước quá trình khởi tạo. Nếu CMD8 reject với illigal
    command error (0x05) thì card là SDC version 1 hoặc MMC version 3. Nếu
    accept thì response R7 sẽ được trả về. 12 bit thấp nhất có giá trị là
    0x1AA thì card sẽ là SDC version 2 và nó có thể hoạt động ở mức điện áp
    là 2.7 tới 3.6V. Nếu không phải các trường hợp này, thì card bị reject.
    */

    {
        if (SDCARD_WaitNotBusy() < 0)
        {
            SD_DESELECT();
            return;
        }

        uint8_t res[4] = {0};
        uint8_t cmd[] = {
            0x40 | 0x08 /* CMD8 */, 0x00, 0x00, 0x01, 0xAA /* ARG = 0x000001AA */, (0x43 << 1) | 1 /* CRC7 + end bit */
        };

        for (i = 0; i < CMD_SIZE; ++i)
        {
            SPI_WriteByte(SPI_HANDLER, cmd[i]);
        }

        res[0] = SDCARD_ReadR1();

        if (res[0] != SD_IDLE_STATE)
        {
            SD_DESELECT();
            return;
        }

        SDCARD_ReadR7(res);
        
        if ((res[2] != 0x01) && (res[3] != 0xAA))
        {
            SD_DESELECT();
            return;
        }
    }

    /*
    Step 4.

    Và sau đó bắt đầu khởi tạo bằng command ACMD41 với cờ HCS flag (bit 30).
    */

    {
        uint8_t res = 0;

        for (i = 0; i < 1000; ++i)
        {
            if (SDCARD_WaitNotBusy() < 0)
            {
                SD_DESELECT();
                return;
            }

            {
                uint8_t cmd[] = {
                    0x40 | 0x37 /* CMD55 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */
                };

                for (i = 0; i < CMD_SIZE; ++i)
                {
                    SPI_WriteByte(SPI_HANDLER, cmd[i]);
                }
            }

            res = SDCARD_ReadR1();
        
            if (res != SD_IDLE_STATE)
            {
                SD_DESELECT();
                return;
            }

            if (SDCARD_WaitNotBusy() < 0)
            {
                SD_DESELECT();
                return;
            }

            {
                uint8_t cmd[] = {
                    0x40 | 0x29 /* ACMD41 */, 0x40, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */
                };

                for (i = 0; i < CMD_SIZE; ++i)
                {
                    SPI_WriteByte(SPI_HANDLER, cmd[i]);
                }
            }

            res = SDCARD_ReadR1();
        
            if (res == 0x00) break;

            if (res != SD_IDLE_STATE)
            {
                SD_DESELECT();
                return;
            }

            delay(1);
        }
    }

    /*
    Bước 5.

    Khi khởi tạo thành công thì đọc thanh ghi OCR register với command CMD58
    và kiểm tra cờ CCS flag (bit 30). Nếu nó được set thì thẻ là thẻ có dung
    lượng cao gọi là SDHC/SDXC.
    */

    {
        if (SDCARD_WaitNotBusy() < 0)
        {
            SD_DESELECT();
            return;
        }

        uint8_t res[4] = {0};
        uint8_t cmd[] = {
            0x40 | 0x3A /* CMD58 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 /* CRC7 + end bit */
        };

        for (i = 0; i < CMD_SIZE; ++i)
        {
            SPI_WriteByte(SPI_HANDLER, cmd[i]);
        }

        res[0] = SDCARD_ReadR1();

        if (res[0] != 0x00)
        {
            SD_DESELECT();
            return;
        }

        SDCARD_ReadR7(res);
        
        if ((res[0] & 0xC0) != 0xC0)
        {
            SD_DESELECT();
            return;
        }
    }

    SD_DESELECT();
    return;
}
