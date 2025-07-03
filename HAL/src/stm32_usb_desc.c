#include "stm32_hal_usb.h"

__ALIGN_BEGIN uint8_t DeviceDesc[MAX_SIZE_DEVICE_DESCRIPTOR] __ALIGN_END = {
    0x12,                       /*bLength */
    DEVICE_TYPE,                /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    USB_DEVICE_CLASS,           /*bDeviceClass*/
    USB_DEVICE_SUB_CLASS,       /*bDeviceSubClass*/
    USB_DEVICE_PROTOCOL,        /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,           /*bMaxPacketSize*/
    LOBYTE(USBD_VID),           /*idVendor*/
    HIBYTE(USBD_VID),           /*idVendor*/
    LOBYTE(USBD_PID_FS),        /*idProduct*/
    HIBYTE(USBD_PID_FS),        /*idProduct*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
};

/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#if SUPPORT_USB_STANDARD
__ALIGN_BEGIN uint8_t ConfigDesc[] __ALIGN_END = {
    /*Configuration Descriptor*/
    MAX_SIZE_CONFIG_DESCRIPTOR, /* bLength: Configuration Descriptor size */
    CONFIGURATION_TYPE,         /* bDescriptorType: Configuration */
    USB_CONFIG_DESC_LEN,        /* wTotalLength:no of returned bytes */
    0x00,
    0x01,                       /* bNumInterfaces: 1 interface */
    0x01,                       /* bConfigurationValue:  Value to use as an argument to the
                                   SetConfiguration() request to select this configuration */
    0x00,                       /* iConfiguration: Index of string descriptor describing the configuration */
    0x80,                       /* bmAttributes */
    0xFA,                       /* MaxPower */

    /* Interface Descriptor */
    0x09,                       /* bLength: Endpoint Descriptor size */
    INTERFACE_TYPE,             /* bDescriptorType: */
    0x00,                       /* bInterfaceNumber: Number of Interface */
    0x00,                       /* bAlternateSetting: Alternate setting */
    0x02,                       /* bNumEndpoints: Two endpoints used */
    0xFF,                       /* bInterfaceClass */
    0xFF,                       /* bInterfaceSubClass: */
    0xFF,                       /* bInterfaceProtocol: */
    0x00,                       /* iInterface: */

    /* Endpoint Out Descriptor */
    0x07,                       /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,              /* bDescriptorType: Endpoint */
    0x01,                       /* bEndpointAddress */
    0x02,                       /* bmAttributes: Bulk */
    USB_MAX_EP_PACKET_SIZE,     /* wMaxPacketSize: */
    0x00,
    0x01,                       /* bInterval */

	/* Endpoint In Descriptor */
    0x07,                       /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,              /* bDescriptorType: Endpoint */
    0x82,                       /* bEndpointAddress */
    0x02,                       /* bmAttributes: Bulk */
    USB_MAX_EP_PACKET_SIZE,     /* wMaxPacketSize: */
    0x00,
    0x01,                       /* bInterval */
};
#endif /* SUPPORT_USB_STANDARD */

#if SUPPORT_USB_CDC
__ALIGN_BEGIN uint8_t ConfigDesc[USB_CONFIG_DESC_LEN] __ALIGN_END = {
    /*Configuration Descriptor*/
    MAX_SIZE_CONFIG_DESCRIPTOR, /* bLength: Configuration Descriptor size */
    CONFIGURATION_TYPE,         /* bDescriptorType: Configuration */
    USB_CONFIG_DESC_LEN,        /* wTotalLength */
    0x00,
    0x02,                       /* bNumInterfaces: 2 interface */
    0x01,                       /* bConfigurationValue:  Value to use as an argument to the
                                   SetConfiguration() request to select this configuration */
    0x00,                       /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,                       /* bmAttributes: self powered */
    0x32,                       /* MaxPower 0 mA */
    
    /*---------------------------------------------------------------------------*/
    
    /*Interface Descriptor */
    0x09,               /* bLength: Interface Descriptor size */
    INTERFACE_TYPE,     /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,               /* bInterfaceNumber: Number of Interface */
    0x00,               /* bAlternateSetting: Alternate setting */
    0x01,               /* bNumEndpoints: One endpoints used */
    0x02,               /* bInterfaceClass: Communication Interface Class */
    0x02,               /* bInterfaceSubClass: Abstract Control Model */
    0x01,               /* bInterfaceProtocol: Common AT commands */
    0x00,               /* iInterface: */
    
    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,
    
    /*Call Management Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    0x01,   /* bDataInterface: 1 */
    
    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */
    
    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */
    
    /*Endpoint 2 Descriptor*/
    0x07,                           /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,                  /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                     /* bEndpointAddress */
    0x03,                           /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SIZE),    /* wMaxPacketSize: */
    HIBYTE(CDC_CMD_PACKET_SIZE),
    0x10,                           /* bInterval: */ 
    /*---------------------------------------------------------------------------*/
    
    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    INTERFACE_TYPE,  /* bDescriptorType: */
    0x01,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */
    
    /*Endpoint OUT Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,      /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                        /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(CDC_MAX_PACKET_SIZE),       /* wMaxPacketSize: */
    HIBYTE(CDC_MAX_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */
    
    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,      /* bDescriptorType: Endpoint */
    CDC_IN_EP,                         /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(CDC_MAX_PACKET_SIZE),       /* wMaxPacketSize: */
    HIBYTE(CDC_MAX_PACKET_SIZE),
    0x00                               /* bInterval: ignore for Bulk transfer */
};
#endif

#if SUPPORT_USB_HID
__ALIGN_BEGIN uint8_t ConfigDesc[USB_CONFIG_DESC_LEN] __ALIGN_END = {
    /*Configuration Descriptor*/
    MAX_SIZE_CONFIG_DESCRIPTOR, /* bLength: Configuration Descriptor size */
    CONFIGURATION_TYPE,         /* bDescriptorType: Configuration */
    USB_CONFIG_DESC_LEN,        /* wTotalLength:no of returned bytes */
    0x00,
    0x01,                       /* bNumInterfaces: 1 interface */
    0x01,                       /* bConfigurationValue:  Value to use as an argument to the
                                   SetConfiguration() request to select this configuration */
    0x00,                       /* iConfiguration: Index of string descriptor describing the configuration */
    0x80,                       /* bmAttributes */
    0xFA,                       /* MaxPower */

    /* Interface Descriptor */
    0x09,                       /* bLength: Interface Descriptor size */
    INTERFACE_TYPE,             /* bDescriptorType: */
    0x00,                       /* bInterfaceNumber: Number of Interface */
    0x00,                       /* bAlternateSetting: Alternate setting */
    0x01,                       /* bNumEndpoints */
    0x03,                       /* bInterfaceClass */
    0x01,                       /* bInterfaceSubClass */
    0x01,                       /* bInterfaceProtocol */
    0x00,                       /* iInterface: */

    /* HID Descriptor */
    0x09,                    // bLength
    0x21,                    // bDescriptorType (HID)
    0x11, 0x01,              // bcdHID 1.11
    0x00,                    // bCountryCode
    0x01,                    // bNumDescriptors
    0x22,                    // bDescriptorType (Report)
    LOBYTE(MAX_SIZE_REPORT_DESCRIPTOR), // wDescriptorLength
    HIBYTE(MAX_SIZE_CONFIG_DESCRIPTOR),

	/* Endpoint In Descriptor */
    0x07,                       /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,              /* bDescriptorType: Endpoint */
    HID_IN_EP,                  /* bEndpointAddress */
    0x03,                       /* bmAttributes: Interrupt */
    HID_MAX_SIZE_REPORT,        /* wMaxPacketSize */
    0x00,
    0x0A,                       /* bInterval */
};
#endif /* SUPPORT_USB_HID */

