#include <string.h>
#include "ethernet.h"
#include "net_arp.h"
#include "net_ip.h"

#define NET_FRAME_MIN_LEN       60
#define NET_FRAME_MAX_LEN       1514

uint8_t eth_broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t eth_zero[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

net_state_t eth_output(uint8_t* payload, uint32_t payload_len)
{
    net_drv_msg_t msg = {0};
    net_state_t res = NET_OK;
    net_ctrl_t* net_ctrl_p = NULL;

    net_ctrl_p = net_get_ctrl();

    if (net_ctrl_p->drv == NULL)
    {
        res = NET_ERR_NULL_HANDLE;
        goto end;
    }

    if (payload_len > NET_FRAME_MAX_LEN)
    {
        res = NET_ERR_FRAME_TOO_LONG;
        goto end;
    }

    memcpy(net_ctrl_p->pbuf, payload, payload_len);

    if (payload_len < NET_FRAME_MIN_LEN)
    {
        memset(&net_ctrl_p->pbuf[payload_len], 0x00, NET_FRAME_MIN_LEN - payload_len);
        payload_len = NET_FRAME_MIN_LEN;
        res = NET_WARN_PADDING_ADDED;
    }

    /* Start send packet */
    msg.data = net_ctrl_p->pbuf;
    msg.data_len = payload_len;
    res = net_ctrl_p->drv(NET_DRV_OUTPUT, &msg);

end:
    return res;
}

net_state_t eth_input(const uint8_t* frame, uint32_t frame_len)
{
    uint16_t    eth_type = 0;
    net_state_t res = NET_OK;

    if (frame_len > NET_FRAME_MAX_LEN)
    {
        res = NET_ERR_FRAME_TOO_LONG;
        goto end;
    }

    eth_type = (frame[12] << 8) | frame[13];

    DEBUG(LOG_INFO, "eth", "dst=%02X:%02X:%02X:%02X:%02X:%02X type=0x%04X len=%d\r\n",
        frame[0],frame[1],frame[2],frame[3],frame[4],frame[5],
        eth_type, frame_len
    );

    switch (eth_type)
    {
        case NET_TYPE_ARP:
            arp_input(frame, frame_len);
            break;

        case NET_TYPE_IP:
            ip_input(frame, frame_len);
            break;

        default:
            break;
    }

end:
    return res;
}
