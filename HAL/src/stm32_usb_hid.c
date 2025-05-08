#include "stm32_usb_hid.h"

__ALIGN_BEGIN uint8_t reportDescriptor[MAX_SIZE_REPORT_DESCRIPTOR] __ALIGN_END = {
    0x06, 0x00, 0xFF,   // Usage Page = 0xFF00 (Vendor Defined Page 1)
    0x09, 0x01,         // Usage (Vendor Usage 1)
    0xA1, 0x01,         // Collection (Application)
    0x19, 0x01,         //      Usage Minimum
    0x29, 0x40,         //      Usage Maximum
    0x15, 0x01,         //      Logical Minimum
    0x25, 0x40,         //      Logical Maximum
    0x75, 0x08,         //      Report Size: 8-bit field size
    0x95, 0x40,         //      Report Count
    0x81, 0x00,         //      Input (Data, Array, Abs)
    0x19, 0x01,         //      Usage Minimum
    0x29, 0x40,         //      Usage Maximum
    0x91, 0x00,         //      Output (Data, Array, Abs)
    0xC0,               // End Collection
};

