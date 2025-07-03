#ifndef __USB_HID__
#define __USB_HID__

#include <stdint.h>
#include <string.h>
#include "stm32_hal_util.h"

#define SUPPORT_USB_HID_KEYBOARD                1
#define SUPPORT_USB_HID_MOUSE                   O
#define SUPPORT_USB_HID_CUSTOM                  0

#define REPORT_DESCRIPTOR_TYPE                  0x22

// Endpoint
#define HID_IN_EP                               0x81
#define HID_OUT_EP                              0x00
#define HID_MAX_PACKET_SIZE                     0x40  /* Endpoint IN & OUT Packet size */        
#define HID_EP_IN_MAX_SIZE                      0x40
#define HID_EP_OUT_MAX_SIZE                     0x40

#define HID_GET_REPORT                          0x01
#define HID_GET_IDLE                            0x02
#define HID_GET_PROTOCOL                        0x03
#define HID_SET_REPORT                          0x09
#define HID_OUTPUT_REPORT                       0x02
#define HID_SET_IDLE                            0x0A
#define HID_SET_PROTOCOL                        0x0B

// Keyboard code
#define KEYBOARD_LEFT_CONTROL                   0x01
#define KEYBOARD_LEFT_SHIFT                     0x02
#define KEYBOARD_LEFT_ALT                       0x04
#define KEYBOARD_LEFT_GUI                       0x08
#define KEYBOARD_RIGHT_CONTROL                  0x10
#define KEYBOARD_RIGHT_SHIFT                    0x20
#define KEYBOARD_RIGHT_ALT                      0x40
#define KEYBOARD_RIGHT_GUI                      0x80

#if SUPPORT_USB_HID_KEYBOARD
#define MAX_SIZE_REPORT_DESCRIPTOR              0x3F
#define HID_MAX_SIZE_REPORT                     0x08

extern void HID_SendKey(uint8_t modifier, uint8_t keycode);
extern void HID_SendCommandList(void);
#endif /* SUPPORT_USB_HID_KEYBOARD */

#if SUPPORT_USB_HID_CUSTOM
#define MAX_SIZE_REPORT_DESCRIPTOR              0x1C
#define HID_MAX_SIZE_REPORT                     0x40
#endif /*SUPPORT_USB_HID_CUSTOM*/

#undef USB_CONFIG_DESC_LEN
#define USB_CONFIG_DESC_LEN                     34

#undef USB_CLASS_SETUP_CALLBACK
#define USB_CLASS_SETUP_CALLBACK                HID_ControlHandler

#undef USB_EP0_OUT_HANDLER
#define USB_EP0_OUT_HANDLER                     HID_EP0_OUT

#undef USB_CLASS_ENDPOINT_INIT
#define USB_CLASS_ENDPOINT_INIT                 HID_EndpointInit

#undef USB_EP1_IN_HANDLER
#define USB_EP1_IN_HANDLER                      HID_EP1_IN

extern uint8_t usb_enum_flag;
extern uint8_t reportDescriptor[MAX_SIZE_REPORT_DESCRIPTOR];

extern void HID_EP1_IN(void);
extern void HID_EndpointInit(void);
extern void HID_EP0_OUT(void* req, uint8_t* buf);
extern uint8_t HID_ControlHandler(uint8_t req, uint8_t** usb_buffer);
extern uint8_t HID_SendReport(uint8_t* data);
extern uint8_t HID_ReceiveReport(uint8_t* data);

#endif /* __USB_HID__ */

