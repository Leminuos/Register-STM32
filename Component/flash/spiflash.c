#include "spiflash.h"

static W25QXX_Status stsreg;
static uint8_t stscmd[] =
{
    0x05,   // Read Status Register 1
    0x01,   // Write Status Register 1
    0x35,   // Read Status Register 2
    0x31,   // Write Status Register 2
    0x15,   // Read Status Register 3
    0x11    // Write Status Register 3
};

void W25QXX_GetDeviceID(uint8_t* ret)
{

    GPIOB->ODR.BITS.ODR12 = 0x00;

    #if MODE_DEVICE_ID == 0x05
        int8_t i = 0;

        SPI_Transfer(SPI_FLASH_INSTANCE, 0x4B);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);

        for (i = 7; i >= 0; i--)
        {
            ret[i] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
        }
    #elif MODE_DEVICE_ID == 0x04
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x9F);
        ret[2] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);  /* Capacity */
        ret[1] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);  /* Memory Type */
        ret[0] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
    #elif MODE_DEVICE_ID == 0x03
        SPI_Transfer(SPI_FLASH_INSTANCE, 0xAB);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        ret[0] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
    #elif MODE_DEVICE_ID == 0x02
    #elif MODE_DEVICE_ID == 0x01
    #else
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x90);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
        ret[1] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
        ret[0] = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
    #endif  /* JEDEC_ID */

    GPIOB->ODR.BITS.ODR12 = 0x01;
}

uint8_t W25QXX_ReadStatus(uint8_t reg)
{
    uint8_t cmd, sts;

    cmd = stscmd[reg * 2];

    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, cmd);
    sts = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    stsreg.word[reg] = sts;

    return sts;
}

FLASH_RET_STS W25QXX_ReadData(uint32_t addr, uint8_t* ret, uint32_t len)
{
    // Xác nhận ghi đã hoàn thành
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == FLASH_BUSY)
        return FLASH_BUSY;

    GPIOB->ODR.BITS.ODR12 = 0x00;
    
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x03);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    #if FAST_READ
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
    #endif /* FAST_READ */

    while (len--)
    {
        *ret = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);
        ret++;
    }

    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_ReadByte(uint32_t addr, uint8_t *ret)
{
    // Xác nhận ghi đã hoàn thành
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == FLASH_BUSY)
        return FLASH_BUSY;

    GPIOB->ODR.BITS.ODR12 = 0x00;
    
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x03);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    #if FAST_READ
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x00);
    #endif /* FAST_READ */

    *ret = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);

    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_WriteStatus(uint8_t res, uint8_t val)
{
    uint8_t cmd = 0;

    cmd = stscmd[res * 2 + 1];

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, cmd);
    SPI_Transfer(SPI_FLASH_INSTANCE, val);

    GPIOB->ODR.BITS.ODR12 = 0x01;
    // Ghi trong khoảng thời gian tW = 15ms

    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

// Khi power off hoặc thực hiện software reset hoặc hardware reset thì các giá trị bit volatile Status Register bit
// sẽ bị mất và các giá trị bit non-volatile Status Register bit sẽ được restore.
// Lệnh Write Enable for Volatile Status Register không set bit Write Enable Latch (WEL).
void W25QXX_WriteEnable(uint8_t vol)
{
    GPIOB->ODR.BITS.ODR12 = 0x00;

    if (vol == STS_NON_VOl)
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x06);
    else
        SPI_Transfer(SPI_FLASH_INSTANCE, 0x50);

    GPIOB->ODR.BITS.ODR12 = 0x01;
}

void W25QXX_WriteDisable(void)
{
    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x04);
    GPIOB->ODR.BITS.ODR12 = 0x01;
}

// Lệnh này set tất cả sector được chỉ định về 0xFF trong khoảng thời gian tSE.
FLASH_RET_STS W25QXX_EraseSector(uint16_t sector)
{
    uint32_t addr;

    addr = sector * SECTOR_SIZE;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x20);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    GPIOB->ODR.BITS.ODR12 = 0x01;

    // Xoá trong khoảng thời gian tSE = 400ms

    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_EraseBlock32KB(uint8_t block)
{
    uint32_t addr;

    addr = block * BLOCK_SIZE_32KB;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    // Check addressed page is protected

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x52);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    GPIOB->ODR.BITS.ODR12 = 0x01;

    // Xoá trong khoảng thời gian tBE1 = 1600ms

    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_EraseBlock64KB(uint8_t block)
{
    uint32_t addr;

    addr = block * BLOCK_SIZE_64KB;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0xD8);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    GPIOB->ODR.BITS.ODR12 = 0x01;

    // Xoá trong khoảng thời gian tBE1 = 2000ms
    
    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_ChipErase(void)
{
    // Vì mặc định các bit Block Protect Bits bp[0-2] là 100
    // => có một vài block được protected
    // => Erase chip sẽ bị ignore
    
    stsreg.word[STS_REG_3] = W25QXX_ReadStatus(STS_REG_3);
    stsreg.bits.wps = 0x01;
    W25QXX_WriteStatus(STS_REG_3, stsreg.word[STS_REG_3]);

    W25QXX_GlobalUnLock();

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x60);

    GPIOB->ODR.BITS.ODR12 = 0x01;

    // Xoá trong khoảng thời gian tCE = 200s

    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    stsreg.bits.wps = 0x00;
    W25QXX_WriteStatus(STS_REG_3, stsreg.word[STS_REG_3]);

    return FLASH_SUCCESS;
}

// Lệnh này cho phép ghi từ một byte đến 256 byte data vào các ô nhớ đã bị erase trước đó
FLASH_RET_STS W25QXX_WriteData(uint32_t addr, uint8_t* buff, uint32_t len)
{
    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x02);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    while (len--)
    {
        SPI_Transfer(SPI_FLASH_INSTANCE, *buff);
        buff++;
    }

    GPIOB->ODR.BITS.ODR12 = 0x01;
    
    // Write trong khoảng 3ms
    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

// Tương tự như lệnh write data
// Có 3 thanh ghi Security 256 byte => 3 * 256 byte thanh ghi Security
// Với OTP nó cho phép các thanh ghi Security chỉ ghi 1 lần, những lần ghi sau sẽ bị ignore
FLASH_RET_STS W25QXX_WriteDataWithSecurity(uint32_t addr, uint8_t* buff, uint32_t len)
{
    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x42);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    while (len--)
    {
        SPI_Transfer(SPI_FLASH_INSTANCE, *buff);
        buff++;
    }

    GPIOB->ODR.BITS.ODR12 = 0x01;
    
    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_WriteByte(uint32_t addr, uint8_t data)
{
    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x02);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, data);

    GPIOB->ODR.BITS.ODR12 = 0x01;
    
    // Write trong khoảng 3ms
    // Xác nhận ghi đã hoàn thành
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_BUSY) == 0)
            break;
    }

    return FLASH_SUCCESS;
}

// Low power mode, lệnh này sẽ ignore cả Read Status Register
void W25QXX_PowerDown(void)
{
    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0xB9);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    // vào trạng thái power down trong khoảng thời gian tDP = 3us
    // Khi ở trạng thái này, mọi lệnh đều bị ignore trừ lệnh release power down.
}

void W25QXX_ReleasePowerDown(void)
{
    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0xAB);
    GPIOB->ODR.BITS.ODR12 = 0x01;
    // vào trạng thái hoạt động bình thường trong khoảng thời gian tRES1 = 3us
}

/* Erase/Program Suspend instruction
 * Các lệnh Write Status Register instruction và Erase instructions không được thực thi trong khi Erase Suspend.
 * Erase Suspend chỉ có tác dụng với các lệnh erase sector hoặc block => nếu trong quá trình Chip Erase thì
 * lệnh này bị ignore.
 * Các lệnh Write Status Register instruction và Program instructions không được thực thi trong khi Program Suspend.
 * Program Suspend chỉ có tác dụng trong quá trình Page Program và Quad Page Program.
 * Nếu bit SUS = 1 và BUSY = 0 thì lệnh Suspend thì sẽ không được thực thi.
 * Power off bất ngờ trong khi Erase/Program Suspend sẽ reset và release suspend state.
 * -> Data trong các page, sector hoặc block đang suspend có thể bị corrupt -> PHẢI LƯU Ý CHỖ NÀY.
 * */
void W25QXX_EraseSuspend(void)
{
    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x75);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    // Cần thời gian tối đa tSUS = 20us để suspend hoạt động erase hoặc program. Trong khoảng thời gian này bit BUSY
    // trong Status Register sẽ bị clear. Ngay khi thực hiện Erase/Program Suspend thì bit SUS sẽ được set lên 1.
}

/* Erase/Program Resume instruction
 * Lệnh này dùng để resume hoạt động erase hoặc program đã bị suspend trước đó.
 * Lệnh này chỉ hợp lệ khi SUS = 1 và BUSY = 0.
 * Khi thực thi, bit SUS sẽ bị clear và bit BUSY sẽ được set lên 1 trong khoảng 200ns.
 * Lệnh Erase/Program Suspend kế tiếp không được thực thi trong khoảng thời gian tối thiểu tSUS
 * với lệnh Resume trước đó.
 * */
void W25QXX_EraseResume(void)
{
    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x75);
    GPIOB->ODR.BITS.ODR12 = 0x01;
}

/* Enable Reset và Reset Device
 * Lệnh này nhằm thực hiện reset device thay thế cho pin RESET.
 * Khi lệnh được thực thi, các hoạt động đang diễn ra của device
 * sẽ bị huỷ và mất hết các volatile setting.
 * Bất kỳ lệnh nào khác ngoài lệnh Reset Device sau khi Enable Reset
 * sẽ vô hiệu hoá trạng thái Reset Enable.
 * */
void W25QXX_ResetDevice(void)
{
    // Check bit Busy và Sus trước khi Reset để tránh làm hỏng data
    stsreg.word[STS_REG_1] = W25QXX_ReadStatus(STS_REG_1);
    if (stsreg.bits.busy == 1 && stsreg.bits.sus)
    {
        W25QXX_EraseResume();
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x66);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x99);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    // Khi lệnh Reset được thực thi, device sẽ mất khoảng tRST=30us để reset.
    // Trong thời gian này, không có lệnh nào được chấp nhận.
}

// Individual Block/Sector protection chỉ valid khi bit WPS = 1.
// Mặc định, tất cả các bit Individual Block/Sector lock được set thành 1 khi power on
// => tất cả memory đều được bảo vệ.

FLASH_RET_STS W25QXX_BlockSectorLock(uint32_t addr)
{
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WPS) == FLASH_WPS)
        return FLASH_WPS;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x36);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_BlockSectorUnLock(uint32_t addr)
{
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WPS) == FLASH_WPS)
        return FLASH_WPS;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x39);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));

    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

// If LSB = 1, the corresponding block/sector is locked; if LSB=0, the corresponding block/sector is unlocked
FLASH_RET_STS W25QXX_ReadBlockSectorLock(uint32_t addr, uint8_t* ret)
{
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WPS) == FLASH_WPS)
        return FLASH_WPS;

    GPIOB->ODR.BITS.ODR12 = 0x00;

    SPI_Transfer(SPI_FLASH_INSTANCE, 0x3D);
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 16) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, ((addr >> 8) & 0xFF));
    SPI_Transfer(SPI_FLASH_INSTANCE, (addr & 0xFF));
    *ret = SPI_Transfer(SPI_FLASH_INSTANCE, 0xFF);

    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_GlobalLock(void)
{
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WPS) == FLASH_WPS)
        return FLASH_WPS;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x7E);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

FLASH_RET_STS W25QXX_GlobalUnLock(void)
{
    if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WPS) == FLASH_WPS)
        return FLASH_WPS;

    // Write Enable
    W25QXX_WriteEnable(STS_NON_VOl);

    // Xác nhận bit Write Enable Latch
    while(1)
    {
        if((W25QXX_ReadStatus(STS_REG_1) & FLASH_WEL) == FLASH_WEL)
            break;
    }

    GPIOB->ODR.BITS.ODR12 = 0x00;
    SPI_Transfer(SPI_FLASH_INSTANCE, 0x98);
    GPIOB->ODR.BITS.ODR12 = 0x01;

    return FLASH_SUCCESS;
}

// Check addressed page is protected
