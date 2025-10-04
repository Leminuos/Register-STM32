#ifndef __NET_UDP_H__
#define __NET_UDP_H__

#include "net.h"

net_state_t udp_input(const uint8_t* payload, uint16_t payload_len);
net_state_t udp_output(const uint8_t *des_ip,uint16_t des_port, uint8_t* payload, uint16_t payload_len);

#endif /* __NET_UDP_H__ */

