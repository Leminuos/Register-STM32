#include "enc.h"
#include "enc_cfg.h"
#include "enc_def.h"

static uint8_t s_cur_bank = 0;
static uint8_t s_enc_init_suc = 0;
static uint16_t s_next_rx_packet_ptr = 0;

static void enc_auto_bank(uint8_t addr);
static uint8_t enc_read_op(uint8_t op, uint8_t addr);
static void enc_write_op(uint8_t op, uint8_t addr, uint8_t data);

static inline uint8_t enc_read_reg(uint8_t addr)
{
    enc_auto_bank(addr);
    return enc_read_op(ENC_OP_RCR, addr);
}

static inline void enc_write_reg(uint8_t addr, uint8_t data)
{
    enc_auto_bank(addr);
    enc_write_op(ENC_OP_WCR, addr, data);
}

static inline void enc_write16_reg(uint8_t addr, uint16_t data)
{
    enc_write_reg(addr, (data & 0xFF));
    enc_write_reg(addr + 1, (data >> 8));
}

static inline void enc_bfset(uint8_t addr, uint8_t mask)
{
    enc_write_op(ENC_OP_BFS, addr, mask);
}

static inline void enc_bfclr(uint8_t addr, uint8_t mask)
{
    enc_write_op(ENC_OP_BFC, addr, mask);
}

static void enc_read_buf(uint8_t* dst, uint16_t len)
{
    enc_cs_low();
    enc_spi_transfer(ENC_OP_RBM);
    while (len--) *dst++ = enc_spi_transfer(0x00);
    enc_cs_high();
}

static void enc_write_buf(const uint8_t* src, uint16_t len)
{
    enc_cs_low();
    enc_spi_transfer(ENC_OP_WBM);
    while (len--) enc_spi_transfer(*src++);
    enc_cs_high();
}

static uint8_t enc_read_op(uint8_t op, uint8_t addr)
{
    uint8_t ret;

    enc_cs_low();
    enc_spi_transfer(op | (addr & 0x1F));
    if (ENC_IS_MAC_MII(addr)) enc_spi_transfer(0x00);
    ret = enc_spi_transfer(0x00);
    enc_cs_high();

    return ret;
}

static void enc_write_op(uint8_t op, uint8_t addr, uint8_t data)
{
    enc_cs_low();
    enc_spi_transfer(op | (addr & 0x1F));
    enc_spi_transfer(data);
    enc_cs_high();
}

static void enc_auto_bank(uint8_t addr)
{
    uint8_t bank = 0;

    // Các thanh ghi common
    if ((addr & 0x1F) >= 0x1B) return;

    bank = (addr >> 5) & 0x03;

    if (s_cur_bank != bank)
    {
        enc_bfclr(ENC_REG_ECON1, ENC_ECON1_BSEL_MASK);
        enc_bfset(ENC_REG_ECON1, bank);
        s_cur_bank = bank;
    }
}

static uint16_t enc_read_phy(uint8_t addr)
{
    uint16_t data = 0;

    enc_write_reg(ENC_REG_MIREGADR, addr);
    enc_write_reg(ENC_REG_MICMD, ENC_MICMD_MIIRD);

    /* Đợi MISTAT.BUSY = 0 */
    while (1)
    {
        uint8_t reg = enc_read_reg(ENC_REG_MISTAT);
        if ((reg & ENC_MISTAT_BUSY) == 0) break;
    }

    enc_write_reg(ENC_REG_MICMD, 0x00);
    data = enc_read_reg(ENC_REG_MIRDL) + ((uint16_t)(enc_read_reg(ENC_REG_MIRDH) << 8));

    return data;
}

static void enc_write_phy(uint8_t addr, uint16_t data)
{
    enc_write_reg(ENC_REG_MIREGADR, addr);
    enc_write16_reg(ENC_REG_MIWRL, data);

    while (1) {
        uint8_t reg = enc_read_reg(ENC_REG_MISTAT);
        if ((reg & ENC_MISTAT_BUSY) == 0) break;
    }
}

void enc_soft_reset(void)
{
    uint32_t reg = 0;

    enc_cs_low();
    enc_spi_transfer(ENC_OP_SRC);
    enc_cs_high();

    enc_delay_ms(2);

    /* Chờ bit ESTAT.CLKRDY = 1 */
    while (1)
    {
        reg = enc_read_reg(ENC_REG_ESTAT);

        if ((reg != 0xFF) && (reg & ENC_ESTAT_CLKRDY)) break;
        enc_delay_ms(1);
    }
}

int enc_read_packet(uint8_t* buff, uint16_t buflen)
{
    int out_len = 0;
    uint8_t rx_cnt = 0;
    uint8_t header_packet[6] = {0};
    uint16_t len_packet = 0;
    uint16_t status_packet = 0;

    if (s_enc_init_suc)
    {
        rx_cnt = enc_read_reg(ENC_REG_EPKTCNT);
        
        if (rx_cnt)
        {
            /* 1. Đặt con trỏ RX vào vị trí bắt đầu gói tin */
            enc_write16_reg(ENC_REG_ERDPTL, s_next_rx_packet_ptr);
    
            /* 2. Đọc 6 byte header:
             *  + 2 byte next packet pointer
             *  + 2 byte length packet
             *  + 2 byte status packet
             */
            enc_read_buf(header_packet, 6);
            s_next_rx_packet_ptr = header_packet[0] | header_packet[1] << 8;
            len_packet = header_packet[2] | header_packet[3] << 8;
            status_packet = header_packet[4] | header_packet[5] << 8;
    
            /* 3. Khi receive ok thì đọc dữ liệu gói. */
            len_packet = len_packet - 4; // 4 byte checksum
            if (len_packet > buflen) len_packet = buflen;
            if ((status_packet & ENC_RSV_RECEIVED_OK))
            {
                enc_read_buf(buff, len_packet);
                out_len = len_packet;
            }
    
            /* 4. Đặt con trỏ ERXRDP  tới vị trí gói tin tiếp theo */
            if ((s_next_rx_packet_ptr - 1) > ENC_RX_END) enc_write16_reg(ENC_REG_ERXRDPTL, ENC_RX_END);
            else enc_write16_reg(ENC_REG_ERXRDPTL, s_next_rx_packet_ptr - 1);
    
            /* 5. Set bit ECON2.PKTDEC để EPKTCNT giảm đi 1 */
            enc_bfset(ENC_REG_ECON2, ENC_ECON2_PKTDEC);
        }
    }

    return out_len;
}

int enc_write_packet(uint8_t* buf, uint16_t buflen)
{
    if (s_enc_init_suc)
    {
        // 1. Chờ quá trình truyền trước đó hoàn tất
        while (enc_read_reg(ENC_REG_ECON1) & ENC_ECON1_TXRTS)
        {
            if (enc_read_reg(ENC_REG_EIR) & ENC_EIR_TXERIF) {
                enc_bfset(ENC_REG_ECON1, ENC_ECON1_TXRST);
                enc_bfclr(ENC_REG_ECON1, ENC_ECON1_TXRST);
                enc_bfclr(ENC_REG_EIR,   ENC_EIR_TXERIF);
                return -1;
            }
        }
    
        // 2. Đặt con trỏ TX tại vị trí bắt đầu truyền
        enc_write16_reg(ENC_REG_EWRPTL, ENC_TX_START);
    
        // 3. Đặt con trỏ tới vị trí cuối của gói
        enc_write16_reg(ENC_REG_ETXNDL, ENC_TX_START + buflen);
    
        // 4. Ghi control byte
        enc_cs_low();
        enc_spi_transfer(ENC_OP_WBM);
        enc_spi_transfer(0x00);
        enc_cs_high();
    
        // 5. Ghi gói vào buffer
        enc_write_buf(buf, buflen);
    
        // 6. Set bit ECON1.TXRTS để bắt đầu truyền gói lên mạng
        enc_bfset(ENC_REG_ECON1, ENC_ECON1_TXRTS);
    
        return 0;
    }

    return -1;
}

int enc_get_link(void)
{
    uint8_t reg = 0;
    reg = enc_read_phy(ENC_PHY_PHSTAT1);

    return (reg & ENC_PHSTAT1_LLSTAT) ? ENC_LINK_ON : ENC_LINK_OFF;
}

int enc_support_crc(void)
{
    uint8_t reg = 0;

    reg = enc_read_reg(ENC_REG_MACON3);

    return (reg & ENC_MACON3_TXCRCEN) ? 1 : 0;
}

int enc_init(const uint8_t* mac)
{
    /* 1. Reset */
    enc_soft_reset();

    if (enc_read_reg(ENC_REG_ERDPTL) != 0xFA) return -1;

    /* 2. Cấu hình kích thước buffer RX và TX */ 
    s_next_rx_packet_ptr = ENC_RX_START;
    enc_write16_reg(ENC_REG_ERXSTL, ENC_RX_START);
    enc_write16_reg(ENC_REG_ERXNDL, ENC_RX_END);
    enc_write16_reg(ENC_REG_ETXSTL, ENC_TX_START);
    enc_write16_reg(ENC_REG_ETXNDL, ENC_TX_END);

    /* 3. Cấu hình con trỏ RX, TX */
    enc_write16_reg(ENC_REG_ERXRDPTL, ENC_RX_END);
    enc_write16_reg(ENC_REG_ERXWRPTL, ENC_RX_START);

    /* 4. Cấu hình lọc bản tin */
    enc_write_reg(ENC_REG_ERXFCON, (ENC_ERXFCON_UCEN | ENC_ERXFCON_BCEN | ENC_ERXFCON_CRCEN));

    /* 5. Cấu hình MAC */
    enc_write_reg(ENC_REG_MACON1, (ENC_MACON1_MARXEN | ENC_MACON1_TXPAUS | ENC_MACON1_RXPAUS));
    enc_write_reg(ENC_REG_MACON3, (ENC_MACON3_TXCRCEN | ENC_MACON3_FRMLNEN | ENC_MACON3_PADCFG2 | ENC_MACON3_PADCFG0));
    enc_write_reg(ENC_REG_MABBIPG, 0x12U);
    enc_write16_reg(ENC_REG_MAMXFLL, 1500U);
    enc_write16_reg(ENC_REG_MAIPGL,  0x0C12U);

    /* 6. MAC address */
    enc_write_reg(ENC_REG_MAADR1, mac[5]);
    enc_write_reg(ENC_REG_MAADR2, mac[4]);
    enc_write_reg(ENC_REG_MAADR3, mac[3]);
    enc_write_reg(ENC_REG_MAADR4, mac[2]);
    enc_write_reg(ENC_REG_MAADR5, mac[1]);
    enc_write_reg(ENC_REG_MAADR6, mac[0]);

    /* 7. Cấu hình PHY */
    enc_write_phy(ENC_PHY_PHCON1, ENC_PHCON1_PRST);
    while (enc_read_phy(ENC_PHY_PHCON1) & ENC_PHCON1_PRST) {};
    enc_write_phy(ENC_PHY_PHCON2, ENC_PHCON2_HDLDIS);

    /* 8. Cấu hình interrupt */
    enc_bfset(ENC_REG_EIE, ENC_EIE_INTIE | ENC_EIE_PKTIE);

    /* 9. Bật nhận packet */
    enc_bfset(ENC_REG_ECON1, ENC_ECON1_RXEN);

    s_enc_init_suc = 1;

    return 0;
}
