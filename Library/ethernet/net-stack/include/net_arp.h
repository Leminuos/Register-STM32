#ifndef __NET_ARP_H__
#define __NET_ARP_H__

#include <stdint.h>
#include "net.h"

#define NET_ARP_GET_MAC_TIMEOUT         2000

int arp_table_check(const uint8_t* ip);
int arp_table_get(const uint8_t* ip, uint8_t* out_mac);
net_state_t arp_output(const uint8_t* target_ip);
net_state_t arp_input(const uint8_t* payload, uint8_t payload_len);

#endif /* __NET_ARP_H__ */
