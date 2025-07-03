#ifndef __USB_CDC__
#define __USB_CDC__

#include <stdint.h>

#define CDC_IN_EP                               0x81  /* EP1 for data IN */
#define CDC_OUT_EP                              0x01  /* EP1 for data OUT */
#define CDC_CMD_EP                              0x82  /* EP2 for CDC commands */
#define CDC_CMD_PACKET_SIZE                     0x08  /* Control Endpoint Packet size */
#define CDC_MAX_PACKET_SIZE                     0x40  /* Endpoint IN & OUT Packet size */
#define MAX_SIZE_COM_CONFIG                     0x07
#define CDC_EP_IN_MAX_SIZE                      CDC_MAX_PACKET_SIZE
#define CDC_EP_OUT_MAX_SIZE                     CDC_MAX_PACKET_SIZE

// CDC class requests
#define CDC_GET_LINE_CODING                     0x21
#define CDC_SET_LINE_CODING                     0x20
#define CDC_SET_LINE_CTLSTE                     0x22
#define CDC_SEND_BREAK                          0x23

#undef USB_CONFIG_DESC_LEN
#define USB_CONFIG_DESC_LEN                     67

#undef USB_CLASS_SETUP_CALLBACK
#define USB_CLASS_SETUP_CALLBACK                CDC_ControlHandler

#undef USB_CLASS_ENDPOINT_INIT
#define USB_CLASS_ENDPOINT_INIT                 CDC_EndpointInit

#undef USB_EP0_OUT_HANDLER
#define USB_EP0_OUT_HANDLER                     CDC_EP0_OUT

#undef USB_EP1_OUT_HANDLER
#define USB_EP1_OUT_HANDLER                     CDC_EP1_OUT

#undef USB_EP1_IN_HANDLER
#define USB_EP1_IN_HANDLER                      CDC_EP1_IN

#define CDC_getBAUD()   CDC_lineCoding.baudrate

typedef struct _CDC_LINE_CODING_TYPE {
    uint32_t baudrate;              // baud rate
    uint8_t  stopbits;              // number of stopbits (0:1bit,1:1.5bits,2:2bits)
    uint8_t  parity;                // parity (0:none,1:odd,2:even,3:mark,4:space)
    uint8_t  databits;              // number of data bits (5,6,7,8 or 16)
} CDC_LINE_CODING_TYPE;

extern void CDC_EP1_IN(void);
extern void CDC_EP1_OUT(void);
extern void CDC_EndpointInit(void);
extern void CDC_EP0_OUT(void* req, uint8_t* buf);
extern uint16_t CDC_Receive(uint8_t* data);
extern void CDC_Transmit(uint8_t* data, uint8_t length);
extern uint8_t CDC_ControlHandler(uint8_t req, uint8_t** usb_buffer);

#endif /* __USB_CDC__ */

