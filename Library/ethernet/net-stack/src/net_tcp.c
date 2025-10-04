#include "net_tcp.h"

net_state_t tcp_input(const uint8_t* payload, uint16_t payload_len)
{
    net_state_t res = NET_OK;

    DEBUG(LOG_INFO, "tcp", "TCP packet input\r\n");

    return res;
}

net_state_t tcp_output(uint8_t* payload, uint16_t payload_len)
{
    net_state_t res = NET_OK;

    DEBUG(LOG_INFO, "tcp", "TCP packet output\r\n");

    return res;
}

