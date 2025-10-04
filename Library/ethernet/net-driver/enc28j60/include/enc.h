#ifndef __ENC_H__
#define __ENC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ============== State code ============== */

typedef enum
{
    /* --- Success state --- */
    ENC_SUCCESS = 0,

    /* --- Link state --- */
    ENC_LINK_OFF,
    ENC_LINK_ON,

    /* --- CRC state */
    ENC_SUPPORT_CRC,
    ENC_NOT_SUPPORT_CRC,

    /* --- Error state --- */
    ENC_ERR_TX_TIMEOUT,              // Gửi frame bị timeout
    ENC_ERR_TX_COLLISION,            // Va chạm trong chế độ half-duplex
    ENC_ERR_TX_ABORTED,              // ENC28J60 báo TX bị huỷ (TXERIF)
    ENC_ERR_RX_OVERFLOW,             // Buffer RX ENC28J60 bị tràn (EIR.OVFL)
    ENC_ERR_RX_CRC_FAIL,             // CRC sai khi nhận (bị drop)
    ENC_ERR_SPI_COMM_FAIL,           // Lỗi giao tiếp SPI với ENC28J60
} enc_state_t;

/* ============== PROTOTYPE ============== */

int enc_get_link(void);
int enc_support_crc(void);
void enc_soft_reset(void);
int enc_init(const uint8_t* mac);
int enc_read_packet(uint8_t* buff, uint16_t buflen);
int enc_write_packet(uint8_t* buf, uint16_t buflen);

#ifdef __cplusplus
}
#endif

#endif /* __ENC_H__ */
