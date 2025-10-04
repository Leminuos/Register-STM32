#ifndef __NET_IP_H__
#define __NET_IP_H__

#include "ethernet.h"

#define ETH_IP_PROTOCOL_ICMP    0x01
#define ETH_IP_PROTOCOL_UDP     0x11
#define ETH_IP_PROTOCOL_TCP     0x06

#define IP_HDR_LEN_NOOPT        20U
#define IP_FLAG_DF              0x4000U
#define IP_FRAG_OFFSET          0x0U
#define IP_DEFAULT_TTL          64U

net_state_t ip_input(const uint8_t* payload, uint16_t payload_len);
net_state_t ip_output(uint8_t* payload, uint16_t payload_len);

#endif /* __NET_IP_H__ */
