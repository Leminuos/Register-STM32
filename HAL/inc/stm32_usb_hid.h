#ifndef __USB_HID__
#define __USB_HID__

#include <stdint.h>
#include <string.h>
#include "stm32_hal_util.h"

#define SUPPORT_USB_HID_KEYBOARD                1
#define SUPPORT_USB_HID_MOUSE                   O
#define SUPPORT_USB_HID_CUSTOM                  0

#define REPORT_DESCRIPTOR_TYPE                  0x22

#undef USB_CONFIG_DESC_LEN
#define USB_CONFIG_DESC_LEN                     34
#define HID_IN_EP                               0x81
#define HID_OUT_EP                              0x00          
#define HID_DATA_FS_IN_PACKET_SIZE              0x40
#define HID_DATA_FS_OUT_PACKET_SIZE             0x40

#define HID_GET_REPORT                          0x01
#define HID_GET_IDLE                            0x02
#define HID_GET_PROTOCOL                        0x03
#define HID_SET_REPORT                          0x09
#define HID_OUTPUT_REPORT                       0x02
#define HID_SET_IDLE                            0x0A
#define HID_SET_PROTOCOL                        0x0B


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

extern uint8_t USB_ENUM_OK;
extern uint8_t reportDescriptor[MAX_SIZE_REPORT_DESCRIPTOR];
extern uint16_t USB_Receive(uint8_t* data, uint8_t ep);
extern uint8_t USB_Transmit(uint8_t* data, uint8_t length, uint8_t ep);

static inline uint8_t HID_SendReport(uint8_t* data)
{
    return USB_Transmit(data, HID_MAX_SIZE_REPORT, (HID_IN_EP & 0x0F));
}

static inline uint8_t HID_ReceiveReport(uint8_t* data)
{
    return USB_Receive(data, HID_OUT_EP);
}

#endif /* __USB_HID__ */

