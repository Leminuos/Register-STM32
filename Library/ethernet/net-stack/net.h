#ifndef __NET_H__
#define __NET_H__

#include <string.h>
#include <stdio.h>
#include "net_utils.h"
#include "net_log.h"
#include "net_cfg.h"
#include "debug.h"

#define NET_MAC_LEN_MAX             6U
#define NET_IPV4_LEN_MAX            4U

#define NET_DRV_SIG_NO_MSG          NULL

typedef enum {
    NET_DRV_INIT,
    NET_DRV_INPUT,
    NET_DRV_OUTPUT
} net_drv_signal_t;

typedef struct {
    uint8_t* data;
    uint32_t data_len;
} net_drv_msg_t;

typedef struct _net_payload_t {
    struct _net_payload_t* next;
    uint8_t* payload;
    uint16_t payload_len;
} net_payload_t;

typedef net_state_t (*net_drv_handle_t)(net_drv_signal_t sig, net_drv_msg_t* msg);

typedef struct PACKED {

    /* IP Address */
    uint8_t ip_addr[NET_IPV4_LEN_MAX];
    uint8_t gateway[NET_IPV4_LEN_MAX];
    uint8_t subnet[NET_IPV4_LEN_MAX];

    /* MAC address */
    uint8_t mac_addr[NET_MAC_LEN_MAX];

    /* Buffer */
    uint8_t* pbuf;

    /* Driver */
    net_drv_handle_t drv;
} net_ctrl_t;

net_ctrl_t* net_get_ctrl(void);
net_state_t net_polling(void);
net_state_t net_parse_mac(const char* mac_str, uint8_t* mac_out);
net_state_t net_parse_ip(const char* ip_str, uint8_t* ip_out);
net_state_t net_init(
    const net_drv_handle_t net_drv,
    const uint8_t* ip,
    const uint8_t* gateway,
    const uint8_t* subnet
);

#endif /* __NET_H__ */
