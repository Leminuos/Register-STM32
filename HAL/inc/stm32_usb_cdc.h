#ifndef __USB_CDC__
#define __USB_CDC__

#include <stdint.h>

#define CDC_IN_EP                               0x81  /* EP1 for data IN */
#define CDC_OUT_EP                              0x01  /* EP1 for data OUT */
#define CDC_CMD_EP                              0x82  /* EP2 for CDC commands */
#define CDC_CMD_PACKET_SIZE                     0x08  /* Control Endpoint Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE             0x40  /* Endpoint IN & OUT Packet size */
#define MAX_SIZE_COM_CONFIG                     0x07
#define CDC_DATA_FS_IN_PACKET_SIZE              CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE             CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_GET_LINE_CODING                     0x21
#define CDC_SET_LINE_CODING                     0x20
#define CDC_SET_LINE_CTLSTE                     0x22
#define CDC_SEND_BREAK                          0x23
#undef USB_CONFIG_DESC_LEN
#define USB_CONFIG_DESC_LEN                     67

#define CDC_getBAUD()   CDC_lineCoding.baudrate

typedef struct _CDC_LINE_CODING_TYPE {
    uint32_t baudrate;              // baud rate
    uint8_t  stopbits;              // number of stopbits (0:1bit,1:1.5bits,2:2bits)
    uint8_t  parity;                // parity (0:none,1:odd,2:even,3:mark,4:space)
    uint8_t  databits;              // number of data bits (5,6,7,8 or 16)
} CDC_LINE_CODING_TYPE;

extern uint16_t USB_Receive(uint8_t* data, uint8_t ep);
extern uint8_t USB_Transmit(uint8_t* data, uint8_t length, uint8_t ep);

static inline void CDC_Transmit(uint8_t* data, uint8_t length)
{
    USB_Transmit(data, length, 0x01);
}

static inline uint16_t CDC_Receive(uint8_t* data)
{
    return USB_Receive(data, 0x01);
}

#endif /* __USB_CDC__ */

