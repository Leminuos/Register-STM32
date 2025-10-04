#include "net.h"
#include "ethernet.h"

#define NET_BUFFER_MAX_SIZE     1518

static net_ctrl_t g_net_ctrl;
static uint8_t net_buffer[NET_BUFFER_MAX_SIZE];
static uint8_t default_ip[NET_IPV4_LEN_MAX] = {192, 168, 1, 1};
static uint8_t default_gateway[NET_IPV4_LEN_MAX] = {192, 168, 1, 1};
static uint8_t default_subnet[NET_IPV4_LEN_MAX] = {255, 255, 255, 0};

static net_state_t net_check_ip(const uint8_t* ip);
static net_state_t net_check_mac(const uint8_t* mac);

net_ctrl_t* net_get_ctrl(void)
{
    return &g_net_ctrl;
}

net_state_t net_parse_mac(const char* mac_str, uint8_t* mac_out)
{
    int count = 0;
    unsigned int mac[NET_MAC_LEN_MAX] = {0};

    if (!mac_str || !mac_out) return NET_ERR_PARAM_INVALID;
    if (strlen(mac_str) < 17) return NET_ERR_PARAM_INVALID;

    count = sscanf( mac_str,
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    &mac[0], &mac[1], &mac[2],
                    &mac[3], &mac[4], &mac[5]
                );

    if (count != 6) return NET_ERR_FORMAT_INVALID;

    for (int i = 0; i < 6; i++)
    {
        if (mac[i] > 0xFF) return NET_ERR_VALUE_OUT_OF_RANGE;
        mac_out[i] = (uint8_t)mac[i];
    }

    return NET_OK;
}

net_state_t net_parse_ip(const char* ip_str, uint8_t* ip_out)
{
    int count = 0;
    unsigned int ip[NET_IPV4_LEN_MAX] = {0};

    if (!ip_str || !ip_out) return NET_ERR_PARAM_INVALID;
    if (strlen(ip_str) < 7) return NET_ERR_PARAM_INVALID;

    count = sscanf( ip_str, "%3u.%3u.%3u.%3u",
                    &ip[0], &ip[1], &ip[2], &ip[3]);

    if (count != 4) return NET_ERR_FORMAT_INVALID;

    for (int i = 0; i < 4; i++)
    {
        if (ip[i] > 255) return NET_ERR_VALUE_OUT_OF_RANGE;
        ip_out[i] = (uint8_t)ip[i];
    }

    return NET_OK;
}

net_state_t net_init(
    const net_drv_handle_t net_drv,
    const uint8_t* ip,
    const uint8_t* gateway,
    const uint8_t* subnet
)
{
    net_state_t res = NET_OK;
    net_drv_msg_t msg = {0};
    uint8_t my_mac[NET_MAC_LEN_MAX] = {0};
    uint8_t my_ip[NET_IPV4_LEN_MAX] = {0};
    uint8_t my_gateway[NET_IPV4_LEN_MAX] = {0};
    uint8_t my_subnet[NET_IPV4_LEN_MAX] = {0};
    
    res = net_parse_mac(NET_MAC, my_mac);
    if (res != NET_OK) goto end;

    res = net_check_mac(my_mac);
    if (res != NET_OK) goto end;

    if (ip != NULL) memcpy(my_ip, ip, NET_IPV4_LEN_MAX);
    else memcpy(my_ip, default_ip, NET_IPV4_LEN_MAX);
    res = net_check_ip(my_ip);
    if (res != NET_OK) goto end;

    if (gateway != NULL) memcpy(my_gateway, gateway, NET_IPV4_LEN_MAX);
    else memcpy(my_gateway, default_gateway, NET_IPV4_LEN_MAX);
    res = net_check_ip(my_gateway);
    if (res != NET_OK) goto end;

    if (subnet != NULL) memcpy(my_subnet, subnet, NET_IPV4_LEN_MAX);
    else memcpy(my_subnet, default_subnet, NET_IPV4_LEN_MAX);
    res = net_check_ip(my_subnet);
    if (res != NET_OK) goto end;

    if (net_drv == NULL)
    {
        res = NET_ERR_NULL_HANDLE;
        goto end;
    }

    g_net_ctrl.pbuf = &net_buffer[0];
    g_net_ctrl.drv = net_drv;
    memcpy(g_net_ctrl.mac_addr, my_mac, NET_MAC_LEN_MAX);
    memcpy(g_net_ctrl.ip_addr, my_ip, NET_IPV4_LEN_MAX);
    memcpy(g_net_ctrl.gateway, my_gateway, NET_IPV4_LEN_MAX);
    memcpy(g_net_ctrl.subnet, my_subnet, NET_IPV4_LEN_MAX);

    msg.data = g_net_ctrl.mac_addr;
    msg.data_len = NET_MAC_LEN_MAX;
    g_net_ctrl.drv(NET_DRV_INIT, &msg);

end:
    return res;
}

net_state_t net_polling(void)
{
    uint16_t len = 0;
    net_drv_msg_t msg = {0};
    net_state_t res = NET_OK;
    net_ctrl_t* net_ctrl_p = NULL;

    net_ctrl_p = net_get_ctrl();

    msg.data = net_ctrl_p->pbuf;
    msg.data_len = NET_BUFFER_MAX_SIZE;

    if (net_ctrl_p->drv == NULL)
    {
        res = NET_ERR_NULL_HANDLE;
        goto end;
    }

    len = net_ctrl_p->drv(NET_DRV_INPUT, &msg);

    if (len)
    {
        res = eth_input(net_buffer, len);
    }

end:
    return res;
}

static net_state_t net_check_mac(const uint8_t* mac)
{
    net_state_t res = NET_OK;
    uint8_t invalid_mac[NET_MAC_LEN_MAX] = {0};

    memset(invalid_mac, 0, 6);
    if (memcmp(mac, invalid_mac, 6) == 0)
    {
        res = NET_ERR_INVALID_MAC;
        goto end;
    }

    memset(invalid_mac, 0xFF, 6);
    if (memcmp(mac, invalid_mac, 6) == 0)
    {
        res = NET_ERR_INVALID_MAC;
        goto end;
    }

end:
    return res;
}

static net_state_t net_check_ip(const uint8_t* ip)
{
    net_state_t res = NET_OK;
    uint8_t invalid_ip[NET_IPV4_LEN_MAX] = {0};

    memset(invalid_ip, 0, 4);
    if (memcmp(ip, invalid_ip, 4) == 0)
    {
        res = NET_ERR_INVALID_IP;
        goto end;
    }

    memset(invalid_ip, 0xFF, 4);
    if (memcmp(ip, invalid_ip, 4) == 0)
    {
        res = NET_ERR_INVALID_IP;
        goto end;
    }

end:
    return res;
}
