#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#include <stdint.h>
#include "net.h"

#define NET_TYPE_ARP            0x0806
#define NET_TYPE_IP             0x0800

extern uint8_t eth_broadcast[];
extern uint8_t eth_zero[];

net_state_t eth_input(const uint8_t* frame, uint32_t frame_len);
net_state_t eth_output(uint8_t* frame, uint32_t frame_len);

#endif /* __ETHERNET_H__ */

