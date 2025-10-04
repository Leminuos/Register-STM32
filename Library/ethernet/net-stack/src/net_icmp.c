#include "net_icmp.h"

#define ICMP_TYPE_REQUEST       0x08
#define ICMP_TYPE_REPLY         0x00

typedef struct PACKED
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

    /* ICMP packet */
    uint8_t  icmp_type;         /* Type */
    uint8_t  icmp_code;         /* Code */
    uint16_t icmp_cs;           /* Checksum */
    uint16_t icmp_id;           /* Identifier */
    uint16_t icmp_seq_num;      /* Sequence number */
} icmp_packet_t;

static net_state_t icmp_reply(const icmp_packet_t* icmp_p);

net_state_t icmp_input(const uint8_t* payload, uint16_t payload_len)
{
    net_state_t    res = NET_OK;
    icmp_packet_t* icmp_p = NULL;

    icmp_p = (icmp_packet_t*) payload;

    switch (icmp_p->icmp_type)
    {
        case ICMP_TYPE_REQUEST:
            res = icmp_reply(icmp_p);
            break;

        default:
            break;
    }

    return res;
}

static net_state_t icmp_reply(const icmp_packet_t* icmp_packet_p)
{
    net_state_t   res = NET_OK;
    icmp_packet_t icmp_reply = {0};

    DEBUG(LOG_INFO, "icmp", "ICMP reply");

    icmp_reply = *icmp_packet_p;

    //hoan vi MAC
    memcpy(icmp_reply.src_mac,icmp_packet_p->des_mac, NET_MAC_LEN_MAX);
    
    //hoan vi IP
    memcpy(icmp_reply.src_ip, icmp_packet_p->dst_ip, NET_IPV4_LEN_MAX);

    icmp_reply.icmp_type = ICMP_TYPE_REPLY;
    icmp_reply.icmp_cs   = net_checksum16(&icmp_reply, sizeof(icmp_packet_t));

    res = eth_output((uint8_t*)&icmp_reply, sizeof(icmp_packet_t));
    
    return res;
}
