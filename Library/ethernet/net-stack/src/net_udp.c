#include "ethernet.h"
#include "net_ip.h"
#include "net_arp.h"
#include "net_udp.h"

static uint16_t ip_next_id;

typedef struct
{
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

    /* UDP packet */
    uint16_t src_port;          /* Source port */
    uint16_t dst_port;          /* Destination port */
    uint16_t length;            /* Length */
    uint16_t checksum;          /* Check sum */
} udp_packet_t;

net_state_t udp_input(const uint8_t* payload, uint16_t payload_len)
{
    net_state_t res = NET_OK;
    DEBUG(LOG_INFO, "udp", "UDP packet input\r\n");

    return res;
}

net_state_t udp_output(const uint8_t *des_ip,uint16_t des_port, uint8_t* payload, uint16_t payload_len)
{
    net_state_t  res = NET_OK;
    uint16_t     total_len = 0;
    uint16_t     flags_frag = 0;
    net_ctrl_t*  net_ctrl_p = NULL;
    udp_packet_t udp_packet;

    DEBUG(LOG_INFO, "udp", "UDP packet output\r\n");

    net_ctrl_p = net_get_ctrl();

    if (arp_table_check(des_ip) < 0)
    {
        res = arp_output(des_ip);
        if (res != NET_OK) goto end;
    }

    if (arp_table_get(des_ip, udp_packet.des_mac) != 0)
    {
        res = NET_ERR_GET_ARP_TABLE;
        goto end;
    }
    
    memcpy(udp_packet.src_mac, net_ctrl_p->mac_addr, NET_MAC_LEN_MAX);
    udp_packet.eth_type  = NET_TYPE_IP;

    udp_packet.ver_hlen  = (uint8_t)(0x04 << 4 | (IP_HDR_LEN_NOOPT / 4));
    udp_packet.service   = 0x00;

    total_len  = (uint16_t)(IP_HDR_LEN_NOOPT + payload_len);
    udp_packet.total_len = htons(total_len);
    udp_packet.id        = htons(ip_next_id++);

    flags_frag = IP_FLAG_DF | IP_FRAG_OFFSET;
    udp_packet.flags_frag = htons(flags_frag);
    udp_packet.ttl        = IP_DEFAULT_TTL;
    udp_packet.protocol   = ETH_IP_PROTOCOL_UDP;
    udp_packet.hdr_cs     = 0;

    memcpy(udp_packet.dst_ip, des_ip, NET_IPV4_LEN_MAX);
    memcpy(udp_packet.src_ip, net_ctrl_p->ip_addr, NET_IPV4_LEN_MAX);
    // udp_packet.hdr_cs    = net_checksum16(&ip_packet, IP_HDR_LEN_NOOPT);

    // eth_output(udp_packet, sizeof());

end:
    return res;
}
