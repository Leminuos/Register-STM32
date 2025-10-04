#include "main.h"
#include "enc.h"
#include "net.h"
#include "net_arp.h"

void TraceInit(void);

int main(void)
{
    init();
    
    while (1)
    {
        loop();
    }
}

net_state_t res;
uint8_t ip[4] = {192, 168, 1, 10};

net_state_t net_drv_handle(net_drv_signal_t sig, net_drv_msg_t* msg)
{
    net_state_t net = NET_OK;

    switch (sig)
    {
        case NET_DRV_INIT:
            if (enc_init(msg->data))
            {
                net = NET_ERR_DRV_FAILED_INIT;
            }
            break;

        case NET_DRV_INPUT:
            if (enc_read_packet(msg->data, msg->data_len) != 0)
            {
                net = NET_ERR_RX_DRIVER_FAIL;
            }

            break;

        case NET_DRV_OUTPUT:
            if (enc_write_packet(msg->data, msg->data_len) != 0)
            {
                net = NET_ERR_TX_DRIVER_FAIL;
            }

            break;
    }
    
    return net;
}

void init(void)
{
    setupHardware();
    SystickConfig(71999);
    TraceInit();
    SPI_Init(SPI2);

    res = net_init(
        net_drv_handle, ip, NULL, NULL
    );

    DEBUG(LOG_INFO, "main", "%s", net_log_err(res));
}

void loop(void)
{
    net_polling();
}

void SystickConfig(uint32_t u32Reload)
{
    /* Cau hinh systick */
    SysTick->VAL = u32Reload;
    SysTick->LOAD = u32Reload;
    SysTick->CTRL = BIT2 | BIT1 | BIT0;
}
