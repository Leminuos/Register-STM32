#include <string.h>
#include "stm32_hal_usb.h"

#if SUPPORT_USB_CDC

#if ENABLE_DEBUG_USB == 1
static const char* TAG = "USB";
#endif /* ENABLE_DEBUG_USB */

static CDC_LINE_CODING_TYPE CDC_LineCoding;
static uint8_t read_count;
static uint8_t ep_buffer[CDC_MAX_PACKET_SIZE];

void CDC_EndpointInit(void)
{
    USB_EndpointInit(USB, ENDPOINT_TYPE_BULK, CDC_IN_EP, 0xC0, CDC_EP_IN_MAX_SIZE);
    USB_EndpointInit(USB, ENDPOINT_TYPE_BULK, CDC_OUT_EP, 0x110, CDC_EP_OUT_MAX_SIZE);
    USB_EndpointInit(USB, ENDPOINT_TYPE_INTERRUPT, CDC_CMD_EP, 0x100, CDC_CMD_PACKET_SIZE);
}

uint8_t CDC_ControlHandler(uint8_t req, uint8_t** usb_buffer)
{
    uint8_t len = 0;

    switch (req)
    {
        case CDC_GET_LINE_CODING:
            len = MAX_SIZE_COM_CONFIG;
            *usb_buffer = (uint8_t*)&CDC_LineCoding;
            break;

        case CDC_SET_LINE_CODING:
            len = 0;
            break;

        case CDC_SET_LINE_CTLSTE:
            len = 0;
            break;

        case CDC_SEND_BREAK:
            len = 0;
            break;

        default:
            len = 0xFF;
            break;
    }

    return len;
}

void CDC_EP0_OUT(void* req, uint8_t* buf)
{
    uint8_t i = 0;
    uint16_t len = 0;
    USB_RequestTypedef* request = (USB_RequestTypedef*) req;

    switch (request->bRequest)
    {
        case CDC_SET_LINE_CODING:
            len = request->wLength > sizeof(CDC_LineCoding) ? sizeof(CDC_LineCoding) : request->wLength;
            
            for (i = 0; i < len; ++i)
            {
                ((uint8_t*)&CDC_LineCoding)[i] = buf[i];
            }

            break;

        default:
            break;
    }
}

extern uint8_t usb_enum_flag;

void CDC_Transmit(uint8_t* data, uint8_t length)
{
    if (usb_enum_flag == 0 || length == 0) return;

    DEBUG_USB(LOG_VERBOSE, TAG, "USB Device send data to host");
    DEBUG_USB(LOG_VERBOSE, TAG, "Count: 0x%02X", length);

    USB_COUNT_TX(0x01) = length;
    USB_WritePMA(USB, USB_ADDR_TX(0x01), data, USB_COUNT_TX(0x01) & 0x3FF);
    USB_SET_STAT_TX(USB, 0x01, STATUS_TX_VALID);
}

uint16_t CDC_Receive(uint8_t* data)
{
    uint16_t count = 0;

    if (read_count)
    {
        count = read_count;
        read_count = 0;
        memcpy(data, ep_buffer, count);
        USB_SET_STAT_RX(USB, 0x01, STATUS_RX_VALID);

        DEBUG_USB(LOG_VERBOSE, TAG, "USB device receive data from host");
        DEBUG_USB(LOG_VERBOSE, TAG, "Count: 0x%02X", count);
    }

    return count;
}

void CDC_EP1_IN(void)
{
    USB_SET_STAT_TX(USB, 0x01, STATUS_TX_NAK);
}

void CDC_EP1_OUT(void)
{
    uint16_t count = USB_COUNT_RX(0x01) & 0x3FF;

    if (count)
    {
        read_count = count;
        USB_ReadPMA(USB, USB_ADDR_RX(0x01), ep_buffer, count);
        USB_SET_STAT_RX(USB, 0x01, STATUS_RX_NAK);
    }
}

#endif /* SUPPORT_USB_CDC */

