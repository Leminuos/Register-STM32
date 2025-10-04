#include "net_ip.h"
#include "net_arp.h"
#include "net_icmp.h"
#include "net_udp.h"
#include "net_tcp.h"

typedef struct PACKED {
    /* Ethernet packet */
    uint8_t  des_mac[NET_MAC_LEN_MAX];      /*< Destination mac >*/
    uint8_t  src_mac[NET_MAC_LEN_MAX];      /*< Destination mac >*/
    uint16_t eth_type;  

    /* IP packet */
    uint8_t  ver_hlen;          /* Version + Header len*/
    uint8_t  service;           /* Service */
    uint16_t total_len;         /* Total length */
    uint16_t id;                /* Identification */
    uint16_t flags_frag;        /* Flags + Fragmentation offset*/
    uint8_t  ttl;               /* Time to live */
    uint8_t  protocol;          /* Protocol */
    uint16_t hdr_cs;            /* Header checksum */
    uint8_t  src_ip[4];         /* Source IP Address */    
    uint8_t  dst_ip[4];         /* Destination IP Address */  
} ip_packet_t;

net_state_t ip_input(const uint8_t* payload, uint16_t payload_len)
{
    net_state_t  res = NET_OK;
    net_ctrl_t*  net_ctrl_p = NULL;
    ip_packet_t* ip_packet_p = NULL;

    net_ctrl_p = net_get_ctrl();
    ip_packet_p = (ip_packet_t*) payload;

    if (memcmp(ip_packet_p->dst_ip, net_ctrl_p->ip_addr, NET_IPV4_LEN_MAX) != 0)
    {
        res = NET_ERR_DST_NOT_FOR_ME;
        goto end;
    }

    DEBUG(LOG_INFO, "IP", "Target ip: %d.%d.%d.%d",
        ip_packet_p->dst_ip[0],
        ip_packet_p->dst_ip[1],
        ip_packet_p->dst_ip[2],
        ip_packet_p->dst_ip[3]
    );

    switch (ip_packet_p->protocol)
    {
        case ETH_IP_PROTOCOL_ICMP:
            res = icmp_input(payload, payload_len);
            break;

        case ETH_IP_PROTOCOL_UDP:
            res = udp_input(payload, payload_len);
            break;

        case ETH_IP_PROTOCOL_TCP:
            res = tcp_input(payload, payload_len);
            break;

        default:
            break;
    }

end:
    return res;
}

net_state_t ip_output(uint8_t* payload, uint16_t payload_len)
{
    net_state_t res = NET_OK;

    res = eth_output(payload, payload_len);

    return res;
}
