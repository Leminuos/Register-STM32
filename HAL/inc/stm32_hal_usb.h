#ifndef __HAL_USB__
#define __HAL_USB__

#include "stm32f103.h"
#include "stm32_hal_util.h"

#define ENABLE_DEBUG_USB                        1

#if ENABLE_DEBUG_USB == 1
#include "debug.h"
#define DEBUG_USB(level, tag, format, ...)      DEBUG(level, tag, format, ##__VA_ARGS__)
#else
#define DEBUG_USB(level, tag, format, ...)
#endif /* ENABLE_DEBUG_USB */

#define HIBYTE(x)                               ((uint8_t)((x & 0xFF00) >>8))
#define LOBYTE(x)                               ((uint8_t)(x & 0x00FF))

#define BTABLE_ADDRESS                          (0x000)
#define USB_ADDR_TX(ep)                         (*(uint16_t*)(((ep*8)*2) + 0x40006000))
#define USB_COUNT_TX(ep)                        (*(uint16_t*)((((ep*8U) + 2U)*2) + 0x40006000))
#define USB_ADDR_RX(ep)                         (*(uint16_t*)((((ep*8) + 4U)*2) + 0x40006000))
#define USB_COUNT_RX(ep)                        (*(uint16_t*)((((ep*8U) + 6U)*2) + 0x40006000))

#define USB_ADDR0_TX                            (USB_ADDR_TX(0))
#define USB_COUNT0_TX                           (USB_COUNT_TX(0))
#define USB_ADDR0_RX                            (USB_ADDR_RX(0))
#define USB_COUNT0_RX                           (USB_COUNT_RX(0))

#define USB_MAX_EP_PACKET_SIZE                  0x40

// Reception status encoding
#define STATUS_RX_DISABLE                       0x00
#define STATUS_RX_STALL                         0x01
#define STATUS_RX_NAK                           0x02
#define STATUS_RX_VALID                         0x03

// Transmission status encoding
#define STATUS_TX_DISABLE                       0x00
#define STATUS_TX_STALL                         0x01
#define STATUS_TX_NAK                           0x02
#define STATUS_TX_VALID                         0x03

// Endpoint type encoding
#define ENDPOINT_TYPE_BULK                      0x00
#define ENDPOINT_TYPE_CONTROL                   0x01
#define ENDPOINT_TYPE_ISO                       0x02
#define ENDPOINT_TYPE_INTERRUPT                 0x03

// Data toggle encoding
#define DATA_TGL_0                              0x00
#define DATA_TGL_1                              0x01

// Standard Device Requests
#define GET_STATUS                              0x00
#define CLEAR_FEATURE                           0x01
#define SET_FEATURE                             0x03
#define SET_ADDRESS                             0x05
#define GET_DESCRIPTOR                          0x06
#define SET_DESCRIPTOR                          0x07
#define GET_CONFIGURATION                       0x08
#define SET_CONFIGURATION                       0x09
#define GET_INTERFACE                           0x0A
#define SET_INTERFACE                           0x0B
#define SYNCH_FRAME                             0x0C

// Request type
#define USB_REQ_TYP_STANDARD                    0x00
#define USB_REQ_TYP_CLASS                       0x01
#define USB_REQ_TYP_VENDOR                      0x02

// Recipient type
#define USB_REQ_RECIP_DEVICE                    0x00
#define USB_REQ_RECIP_INTERACE                  0x01
#define USB_REQ_RECIP_ENDP                      0x02

// Descriptor Types
#define DEVICE_TYPE                             0x01
#define CONFIGURATION_TYPE                      0x02
#define STRING_TYPE                             0x03
#define INTERFACE_TYPE                          0x04
#define ENDPOINT_TYPE                           0x05
#define DEVICE_QUALIFIER_TYPE                   0x06
#define OTHER_SPEED_CONFIGURATION_TYPE          0X07
#define INTERFACE_POWER                         0x08

// String Index
#define USBD_MAX_STR_DESC_SIZ                   512
#define USBD_IDX_LANGID_STR                     0x00 
#define USB_LEN_LANGID_STR_DESC                 0x04
#define USBD_LANGID_STRING                      1033
#define USBD_IDX_MFC_STR                        0x01 
#define USBD_MANUFACTURER_STRING                "STMicroelectronics"
#define USBD_IDX_PRODUCT_STR                    0x02
#define USBD_PRODUCT_STRING_FS                  "Nguyen dep trai"
#define USBD_IDX_SERIAL_STR                     0x03 
#define USBD_SERIALNUMBER_STRING_FS             "00000000001A"
#define USBD_IDX_CONFIG_STR                     0x04 
#define USBD_CONFIGURATION_STRING_FS            "CDC Config"
#define USBD_IDX_INTERFACE_STR                  0x05
#define USBD_INTERFACE_STRING_FS                "CDC Interface"

// Device Descriptor
#define MAX_SIZE_DEVICE_DESCRIPTOR              0x12
#define USB_MAX_EP0_SIZE                        0x40
#define USB_DEVICE_CLASS                        0x00
#define USB_DEVICE_SUB_CLASS                    0x00
#define USB_DEVICE_PROTOCOL                     0x00
#define USBD_VID                                1155
#define USBD_PID_FS                             22336
#define USBD_MAX_NUM_CONFIGURATION              1

// Configuration Descriptor
#define MAX_SIZE_CONFIG_DESCRIPTOR              0x09
#define USB_CONFIG_DESC_LEN                     0x20
#define SUPPORT_USB_CDC

#ifdef SUPPORT_USB_CDC
#include "stm32_usb_cdc.h"
#endif /* SUPPORT_USB_CDC */

#ifdef SUPPORT_USB_HID
#include "stm32_usb_hid.h"
#endif /* SUPPORT_USB_HID */

#define USB_SET_TYPE_TRANSFER(USBx, EP, TYPE)                       \
     do {                                                           \
         register uint16_t wValReg = 0;                             \
         wValReg = (USBx->EPnRp[EP].WORD & 0x8F8F) | ((TYPE) << 9); \
         USBx->EPnRp[EP].WORD = wValReg;                            \
     } while (0)                                                    \

#define USB_SET_ENDPOINT_ADDRESS(USBx, EP)                          \
     do {                                                           \
         register uint16_t wValReg = 0;                             \
         wValReg = (USBx->EPnRp[EP].WORD & 0x8F8F) | (EP);          \
         USBx->EPnRp[EP].WORD = wValReg;                            \
     } while (0)                                                    \

#define USB_SET_STAT_RX(USBx, EP, STS)                          \
    do                                                          \
    {                                                           \
        register uint16_t _wRegVal = 0;                         \
        _wRegVal = (USBx->EPnRp[EP].BITS.STAT_RX ^ STS) << 12;  \
        _wRegVal = _wRegVal | (USBx->EPnRp[EP].WORD & 0x8F8F);  \
        USBx->EPnRp[EP].WORD = _wRegVal;                        \
    } while (0)                                                 \

#define USB_DATA_TGL_RX(USBx, EP, TGL)                          \
    do                                                          \
    {                                                           \
        register uint16_t _wRegVal = 0;                         \
        _wRegVal = (USBx->EPnRp[EP].BITS.DTOG_RX ^ TGL) << 14;  \
        _wRegVal = _wRegVal | (USBx->EPnRp[EP].WORD & 0x8F8F);  \
        USBx->EPnRp[EP].WORD = _wRegVal;                        \
    } while (0)                                                 \

#define USB_SET_STAT_TX(USBx, EP, STS)                          \
    do                                                          \
    {                                                           \
        register uint16_t _wRegVal = 0;                         \
        _wRegVal = (USBx->EPnRp[EP].BITS.STAT_TX ^ STS) << 4;   \
        _wRegVal = _wRegVal | (USBx->EPnRp[EP].WORD & 0x8F8F);  \
        USBx->EPnRp[EP].WORD = _wRegVal;                        \
    } while (0)                                                 \

#define USB_DATA_TGL_TX(USBx, EP, TGL)                          \
    do                                                          \
    {                                                           \
        register uint16_t _wRegVal = 0;                         \
        _wRegVal = (USBx->EPnRp[EP].BITS.DTOG_TX ^ TGL) << 6;   \
        _wRegVal = _wRegVal | (USBx->EPnRp[EP].WORD & 0x8F8F);  \
        USBx->EPnRp[EP].WORD = _wRegVal;                        \
    } while (0)                                                 \

/* Truoc khi su dung hai macro nay thi test xem lieu cac bit toggle trong thanh ghi co bi clear khi set bit bat ky hay khong */
/* Neu co thi su dung macro nay */

#define CLEAR_TRANSFER_TX_FLAG(USBx, EP)                            \
     do {                                                           \
         register uint16_t wValReg = 0;                             \
         wValReg = (USBx->EPnRp[EP].WORD & 0x8F8F) & ~(1 << 7);     \
         USBx->EPnRp[EP].WORD = wValReg;                            \
     } while (0)                                                    \

#define CLEAR_TRANSFER_RX_FLAG(USBx, EP)                            \
     do {                                                           \
         register uint16_t wValReg = 0;                             \
         wValReg = (USBx->EPnRp[EP].WORD & 0x8F8F) & ~(1 << 15);    \
         USBx->EPnRp[EP].WORD = wValReg;                            \
     } while (0)                                                    \

typedef struct
{
    union
    {
        uint8_t byte;

        struct {
            uint8_t recipient   : 5;    /* 0 = Device
                                           1 = Interface
                                           2 = Endpoint
                                           3 = Other
                                           4...31 = Reserved */

            uint8_t type        : 2;    /* 0 = Standard
                                           1 = Class
                                           2 = Vendor
                                           3 = Reserved */

            uint8_t dir         : 1;    /* 0 = Host-to-device
                                           1 = Device-to-hos */
        } bits;
    } bmRequestType;

    uint8_t     bRequest;
    uint16_t    wValue;
    uint16_t    wIndex;
    uint16_t    wLength;
} USB_RequestTypedef;

void USB_PowerOnReset(void);
void USB_ResetCallBack(void);
void USB_TransactionCallBack(void);

#endif /* __HAL_USB__ */
