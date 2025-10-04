#include <string.h>
#include "ethernet.h"
#include "net_arp.h"

#define ETH_ARP_REQUEST     0x0100
#define ETH_ARP_REPLY       0x0200
#define ARP_TABLE_MAX_LEN   10

typedef struct {
    uint8_t ip[4];
    uint8_t mac[6];
} arp_table_t;

typedef struct PACKED {
    /* Ethernet packet */
    uint8_t  des_mac[NET_MAC_LEN_MAX];      /*< Destination mac >*/
    uint8_t  src_mac[NET_MAC_LEN_MAX];      /*< Destination mac >*/
    uint16_t eth_type;                      /*< Ethernet type >*/

    /* ARP packet */
    uint16_t htype;                         /*< Hardware Type >*/
    uint16_t ptype;                         /*< Protocol Type >*/
    uint8_t  hlen;                          /*< Hardware Address Length >*/
    uint8_t  plen;                          /*< Protocol Address Type >*/
    uint16_t opcode;                        /*< Operation Code >*/
    uint8_t  sender_mac[NET_MAC_LEN_MAX];   /*< Sender MAC >*/
    uint8_t  sender_ip[NET_IPV4_LEN_MAX];   /*< Sender IP >*/
    uint8_t  target_mac[NET_MAC_LEN_MAX];   /*< Target MAC >*/
    uint8_t  target_ip[NET_IPV4_LEN_MAX];   /*< Target IP >*/
} arp_packet_t;

static uint8_t     arp_table_idx;
static arp_table_t arp_table[ARP_TABLE_MAX_LEN];

static void arp_table_set(const uint8_t* ip, const uint8_t* mac);

net_state_t arp_output(const uint8_t* target_ip)
{
    net_state_t  res = NET_OK;
    arp_packet_t arp_request = {0};
    net_ctrl_t*  net_ctrl_p = NULL;

    net_ctrl_p = net_get_ctrl();

    /* Ethernet packet */
    memcpy(arp_request.des_mac, eth_broadcast, NET_MAC_LEN_MAX);
    memcpy(arp_request.src_mac, net_ctrl_p->mac_addr, NET_MAC_LEN_MAX);
    arp_request.eth_type = NET_TYPE_ARP;

    /* ARP packet */
    arp_request.htype = 0x0100;
    arp_request.ptype = 0x0008;
    arp_request.hlen = 0x06;
    arp_request.plen = 0x04;
    arp_request.opcode = ETH_ARP_REQUEST;
    memcpy(arp_request.sender_mac, net_ctrl_p->mac_addr, NET_MAC_LEN_MAX);
    memcpy(arp_request.sender_ip, net_ctrl_p->ip_addr, NET_IPV4_LEN_MAX);
    memcpy(arp_request.target_mac, eth_zero, NET_MAC_LEN_MAX);
    memcpy(arp_request.target_ip, target_ip, NET_IPV4_LEN_MAX);

    res = eth_output((uint8_t*)&arp_request, sizeof(arp_packet_t));

    return res;
}

net_state_t arp_input(const uint8_t* payload, uint8_t payload_len)
{
    net_state_t   res = NET_OK;
    arp_packet_t  arp_reply = {0};
    arp_packet_t* arp_frame_p = NULL;
    net_ctrl_t*   net_ctrl_p = NULL;

    arp_frame_p = (arp_packet_t*) payload;
    net_ctrl_p = net_get_ctrl();

    if (memcmp(net_ctrl_p->ip_addr, arp_frame_p->target_ip, 4) != 0)
    {
        res = NET_ERR_DST_NOT_FOR_ME;
        goto end;
    }

    DEBUG(LOG_INFO, "arp", "Target ip: %d.%d.%d.%d",
        arp_frame_p->target_ip[0],
        arp_frame_p->target_ip[1],
        arp_frame_p->target_ip[2],
        arp_frame_p->target_ip[3]
    );

    /* Ethernet packet */
    memcpy(arp_reply.des_mac, arp_frame_p->src_mac, NET_MAC_LEN_MAX);
    memcpy(arp_reply.src_mac, net_ctrl_p->mac_addr, NET_MAC_LEN_MAX);
    arp_reply.eth_type = htons(NET_TYPE_ARP);

    switch(arp_frame_p->opcode)
    {
        case ETH_ARP_REQUEST:
            arp_reply.htype = 0x0100;
            arp_reply.ptype = 0x0008;
            arp_reply.hlen = 0x06;
            arp_reply.plen = 0x04;
            arp_reply.opcode = ETH_ARP_REPLY;
            memcpy(arp_reply.sender_mac, net_ctrl_p->mac_addr, 6);
            memcpy(arp_reply.sender_ip, net_ctrl_p->ip_addr, 4);
            memcpy(arp_reply.target_mac, arp_frame_p->sender_mac, 6);
            memcpy(arp_reply.target_ip, arp_frame_p->sender_ip, 4);

            res = eth_output((uint8_t*)&arp_reply, sizeof(arp_packet_t));
            break;

        case ETH_ARP_REPLY:
            arp_table_set(arp_frame_p->sender_ip, arp_frame_p->sender_mac);
            break;

        default:
            break;
    }

end:
    return res;
}

int arp_table_check(const uint8_t* ip)
{
    uint8_t i = 0;

    for (i = 0; i < ARP_TABLE_MAX_LEN; ++i)
    {
        if (memcmp(ip, arp_table[i].ip, 4) == 0)
        {
            return i;
        }
    }

    return -1;
}

int arp_table_get(const uint8_t* ip, uint8_t* out_mac)
{
    int idx = arp_table_check(ip);
    if (idx < 0) return 1;
    memcpy(out_mac, arp_table[idx].mac, NET_MAC_LEN_MAX);
    return 0;
}

static void arp_table_set(const uint8_t* ip, const uint8_t* mac)
{
    if (arp_table_check(ip) >= 0) return;

    memcpy(arp_table[arp_table_idx].ip, ip, 4);
    memcpy(arp_table[arp_table_idx].mac, mac, 6);
    ++arp_table_idx;
    if (arp_table_idx >= ARP_TABLE_MAX_LEN) arp_table_idx = 0;
}
