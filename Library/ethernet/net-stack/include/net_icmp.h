#ifndef __ICMP_H__
#define __ICMP_H__

#include "net_ip.h"

net_state_t icmp_input(const uint8_t* payload, uint16_t payload_len);

#endif /* __ICMP_H__ */

