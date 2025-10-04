#ifndef __NET_TCP_H__
#define __NET_TCP_H__

#include "net.h"

net_state_t tcp_input(const uint8_t* payload, uint16_t payload_len);
net_state_t tcp_output(uint8_t* payload, uint16_t payload_len);

#endif /* __NET_TCP_H__ */

