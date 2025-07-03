#ifndef __USB_CONFIG__
#define __USB_CONFIG__

#define ENABLE_DEBUG_USB                        1

#define USB_MAX_EP_PACKET_SIZE                  0x40

// Device config
#define USBD_VID                                1155
#define USBD_PID_FS                             22336
#define USBD_MAX_NUM_CONFIGURATION              1

// String config
#define USBD_LANGID_STRING                      1033
#define USBD_MANUFACTURER_STRING                "STMicroelectronics"
#define USBD_PRODUCT_STRING_FS                  "Nguyen dep trai"
#define USBD_SERIALNUMBER_STRING_FS             "00000000001A"
#define USBD_CONFIGURATION_STRING_FS            "CDC Config"
#define USBD_INTERFACE_STRING_FS                "CDC Interface"

// USB Class
#define SUPPORT_USB_CDC                         1
#define SUPPORT_USB_HID                         0

#endif /* __USB_CONFIG__ */

