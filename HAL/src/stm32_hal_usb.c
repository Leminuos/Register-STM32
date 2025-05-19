#include "stm32_hal_usb.h"

static USB_RequestTypedef   DevRequest;
static uint8_t              buffout[64];
uint8_t              EP0_buffer[10];
static uint8_t              *buffin;
static uint8_t              epindex;
static uint8_t              u8Address;
static uint8_t              config;
static uint16_t             bCount;
static uint8_t              ControlState;   /*------------------------------------------
                                             * Bit 7: Error flag                       * 
                                             * Bit 6: Setup data packet length > 0x40  *
                                             * Bit 3: Flag transmitter
                                             * Bit 2: Flag receiver                    *
                                             * Bit 1: Direction                        *
                                             *        1 => OUT                         *
                                             *        0 => IN                          *
                                             * Bit 0: Stage                            *
                                             *        1 => Status Stage                *
                                             *        0 => Data Stage                  *
                                             ------------------------------------------*/

static uint8_t              USB_ENUM_OK;

__ALIGN_BEGIN uint8_t DeviceDesc[MAX_SIZE_DEVICE_DESCRIPTOR] __ALIGN_END = {
    0x12,                       /*bLength */
    DEVICE_TYPE,                /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x02,                       /*bDeviceClass*/
    0x02,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
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

uint8_t ConfigDesc[] = {
    /*Configuration Descriptor*/
    MAX_SIZE_CONFIG_DESCRIPTOR, /* bLength: Configuration Descriptor size */
    CONFIGURATION_TYPE,         /* bDescriptorType: Configuration */
    USB_CDC_CONFIG_DESC_SIZ,    /* wTotalLength:no of returned bytes */
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
    LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */
    
    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    ENDPOINT_TYPE,      /* bDescriptorType: Endpoint */
    CDC_IN_EP,                         /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
    0x00                               /* bInterval: ignore for Bulk transfer */
};

/** USB lang indentifier descriptor. */
__ALIGN_BEGIN uint8_t LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
     USB_LEN_LANGID_STR_DESC,
     STRING_TYPE,
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING)
};

/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

CDC_LINE_CODING_TYPE CDC_LineCoding;

#define MARK_TEST_SIZE      100U

uint32_t mark[MARK_TEST_SIZE];

void SignalTest(uint8_t TestSize)
{
    static uint8_t togbit = 0;
    uint8_t i = 0;

    for (i = 0; i < TestSize; ++i)
    {
        GPIOC->ODR.BITS.ODR13 = 0;
        GPIOC->ODR.BITS.ODR13 = 1;
    }

    if (togbit == 0)
    {
        GPIOC->ODR.BITS.ODR13 = 1;
        togbit = 1;
    }
    else
    {
        GPIOC->ODR.BITS.ODR13 = 0;
        togbit = 0;
    }
}

void MarkTest(uint32_t val)
{
    static uint8_t cnt = 0;

    if (cnt == MARK_TEST_SIZE) return;

    mark[cnt] = val;
    ++cnt;
}

// Read Packet Buffer Memory Address

static void USB_ReadPMA(USB_Typedef* USBx , uint16_t wBufAddrPMA, uint8_t* buff, uint16_t wCount)
{
    uint32_t index          = 0;
    uint32_t nCount         = (wCount + 1) >> 1;
    uint32_t* pBufAddrAPB   = 0;

    /* In the following pages two location addresses are reported: the one to be used by
    application software while accessing the packet memory, and the local one relative to USB
    Peripheral access. To obtain the correct STM32F10xxx memory address value to be used in
    the application software while accessing the packet memory, the actual memory location
    address must be multiplied by two. */

    pBufAddrAPB = (uint32_t*) (wBufAddrPMA*2 + (uint32_t) USBx + 0x400);

    for (index = 0; index < nCount; ++index)
    {
        *((uint16_t*) buff) = *((uint16_t*) pBufAddrAPB);
        pBufAddrAPB++;
        buff = buff + 2;
    }
}

// Write Packet Buffer Memory Address 

static void USB_WritePMA(USB_Typedef* USBx, uint16_t wBufAddrPMA, uint8_t* buff, uint16_t wCount)
{
    uint32_t index          = 0;
    uint32_t nCount         = (wCount + 1) >> 1;
    uint16_t *pBufAddrAPB   = 0, temp1, temp2;

    pBufAddrAPB = (uint16_t*) (wBufAddrPMA*2 + (uint32_t) USBx + 0x400);

    if (buff == NULL) return;

    for (index = 0; index < nCount; ++index)
    {
        temp1 = (uint16_t) (*buff);
        buff++;
        temp2 = temp1 | (((uint16_t) (*buff)) << 8);
        *pBufAddrAPB = temp2;
        pBufAddrAPB = pBufAddrAPB + 2;
        buff++;
    }
}

static uint8_t USBD_GetLen(uint8_t *desc)
{
    uint8_t len = 0;

    while (*desc != '\0')
    {
        ++len;
        ++desc;
    }

    return len;
}

static void USB_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t idx = 0;
  
    if (desc) 
    {
        *len =  USBD_GetLen(desc) * 2 + 2;    
        unicode[idx++] = *len;
        unicode[idx++] = STRING_TYPE;
        
        while (*desc != '\0') 
        {
            unicode[idx++] = *desc++;
            unicode[idx++] =  0x00;
        }
    } 
}

static uint8_t USB_BufferDescTable(uint8_t ep, uint16_t addr, uint16_t count)
{
    uint16_t bCount     = 0;

    // Ghi các giá trị vào thành ghi ADDRn_TX/ADDRn_RX để ngoại vi biết dữ liệu
    // sẽ được lưu và nhận tại đâu.

    if ((ep & 0x80) != RESET)   // Transmission
    {
        ep = ep & 0x0F;
        USB_ADDR_TX(ep) = addr;
    }
    else                        // Reception
    {
        ep = ep & 0x0F;

        if (count > 62U)
        {
            bCount = count >> 6U; 

            //if (bCount & 0x000F) return -1;

            bCount = bCount << 10;
            bCount = bCount | 0x8000;   // Set BL_SIZE
        }
        else
        {
            bCount = (count + 1U) >> 1U;

            //if (bCount == 0) return -1;
            
            bCount = bCount << 10;
        }

        USB_ADDR_RX(ep) = addr;
        USB_COUNT_RX(ep) = bCount;
    }
    
    return 0;
}

static void USB_EndpointInit(USB_Typedef* USBx, uint8_t type, uint8_t addr, uint16_t packetAddr, uint16_t maxPacketSize)
{
    // Set endpoint type
    uint8_t ep = 0;

    ep = addr & 0x7FU;
    USB_SET_TYPE_TRANSFER(USBx, ep, type);
    USB_SET_ENDPOINT_ADDRESS(USBx, ep);

    if ((addr & 0x80) == 0x80)      // IN endpoint
    {
        // Set bit STAT_TX và clear DTOG_TX
        USB_SET_STAT_TX(USBx, ep, STATUS_TX_NAK);
        USB_DATA_TGL_TX(USBx, ep, DATA_TGL_0);
    }
    else
    {
        // Set bit STAT_RX và clear DTOG_RX
        USB_SET_STAT_RX(USBx, ep, STATUS_RX_VALID);
        USB_DATA_TGL_RX(USBx, ep, DATA_TGL_0);
    }

    USB_BufferDescTable(addr, packetAddr, maxPacketSize);
}

static void USB_ProcessSetupStage(USB_Typedef* USBx, uint8_t *buff)
{
    uint16_t            length = 0;
    uint8_t             descType = 0;
    uint8_t             descIdx = 0;
    uint8_t             *bufftmp = NULL;

    ControlState = 0;       /* Default: Next stage is data stage and direction is IN */
    DevRequest = *((USB_RequestTypedef*) buff);
    descType = (uint8_t) ((DevRequest.wValue >> 8) & 0xFF);
    descIdx  = (uint8_t) (DevRequest.wValue & 0xFF);

    if (DevRequest.bmRequestType.bits.type != RESET)
    {
        if (DevRequest.bmRequestType.bits.type == USB_REQ_TYP_CLASS)
        {
            switch (DevRequest.bRequest)
            {
                case CDC_GET_LINE_CODING:
                    bCount = MAX_SIZE_COM_CONFIG;
                    buffin = (uint8_t*)&CDC_LineCoding;
                    USB_ReadPMA(USBx, USB_ADDR0_RX, EP0_buffer, USB_COUNT0_RX & 0x3FF);
                    break;

                case CDC_SET_LINE_CODING:
                    ControlState = ControlState | 0x02; /* Next OUT direction */
                    break;

                case CDC_SET_LINE_CTLSTE:
                    break;

                case CDC_SEND_BREAK:
                    break;

                default:
                    ControlState = ControlState | 0x80;
                    break;
            }
        }
        else if (DevRequest.bmRequestType.bits.type == USB_REQ_TYP_VENDOR)
        {
            /* Manufacturer request */
        }
        else
        {
            ControlState = ControlState | 0x80;
        }
    }
    else
    {
        switch (DevRequest.bRequest)
        {
            case GET_DESCRIPTOR:    
                if (descType == DEVICE_TYPE)
                {
                    // Write Data
                    bCount = MAX_SIZE_DEVICE_DESCRIPTOR;
                    buffin = DeviceDesc;
                }
                else if (descType == CONFIGURATION_TYPE)
                {
                    // Write Data
                    bCount = DevRequest.wLength < USB_CDC_CONFIG_DESC_SIZ ? DevRequest.wLength : USB_CDC_CONFIG_DESC_SIZ;
                    buffin = ConfigDesc;
                }
                else if (descType == DEVICE_QUALIFIER_TYPE)
                {
                    bCount      = 0;
                    buffin      = NULL;
                    ControlState = ControlState | 0x80;
                }
                else if (descType == STRING_TYPE)
                {
                    // USB Language Identifiers
                    // http://www.baiheee.com/Documents/090518/090518112619/USB_LANGIDs.pdf

                    switch (descIdx)
                    {
                        case USBD_IDX_LANGID_STR:
                            bCount = USB_LEN_LANGID_STR_DESC;
                            buffin = LangIDDesc;
                            break;

                        case USBD_IDX_MFC_STR:
                            USB_GetString((uint8_t *)USBD_MANUFACTURER_STRING, StrDesc, &length);
                            bCount = length;
                            buffin = StrDesc;
                            break;

                        case USBD_IDX_PRODUCT_STR:
                            USB_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, StrDesc, &length);
                            bCount = length;
                            buffin = StrDesc;
                            break;

                        case USBD_IDX_SERIAL_STR:
                            USB_GetString((uint8_t *)USBD_SERIALNUMBER_STRING_FS, StrDesc, &length);
                            bCount = length;
                            buffin = StrDesc;
                            break;

                        case USBD_IDX_CONFIG_STR:
                            USB_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, StrDesc, &length);
                            bCount = length;
                            buffin = StrDesc;
                            break;

                        case USBD_IDX_INTERFACE_STR:
                            USB_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, StrDesc, &length);
                            bCount = length;
                            buffin = StrDesc;
                            break;

                        default:
                            bCount      = 0;
                            buffin      = NULL;
                            ControlState = ControlState | 0x80;
                            break;
                    }
                }
                else
                {
                    ControlState = ControlState | 0x80;
                }

                break;
            
            case SET_ADDRESS:
                ControlState = ControlState | 0x01;     /* Next Status Stage */
                u8Address   = DevRequest.wValue & 0xFF;
                bCount      = 0;    // Reset count khi nhận được request SET_ADDRESS
                                    // Nếu không sẽ gặp lỗi không thể set được address
                buffin      = NULL;
                break;

            case GET_CONFIGURATION:
                EP0_buffer[0]   = config;
                buffin          = EP0_buffer;
                bCount          = 1;
                break;
            
            case SET_CONFIGURATION:
                config      = DevRequest.wValue & 0xFF;
                USB_EndpointInit(USB, ENDPOINT_TYPE_BULK, CDC_IN_EP, 0xC0, CDC_DATA_FS_IN_PACKET_SIZE);
                USB_EndpointInit(USB, ENDPOINT_TYPE_BULK, CDC_OUT_EP, 0x110, CDC_DATA_FS_IN_PACKET_SIZE);
                USB_EndpointInit(USB, ENDPOINT_TYPE_INTERRUPT, CDC_CMD_EP, 0x100, CDC_CMD_PACKET_SIZE);
                USB_ENUM_OK = 1;
                break;

            case CLEAR_FEATURE:
                if (DevRequest.bmRequestType.bits.recipient == USB_REQ_RECIP_DEVICE)
                {
                    if ((DevRequest.wValue & 0xFF) == 0x01)
                    {
                        if (ConfigDesc[7] & 0x20)
                        {
                            // wake up
                        }
                        else ControlState = ControlState | 0x80;
                    }
                    else ControlState = ControlState | 0x80;
                }
                else if (DevRequest.bmRequestType.bits.recipient == USB_REQ_RECIP_ENDP)
                {
                    switch (DevRequest.wIndex & 0xFF)
                    {
                        case 0x01:      // EP1 OUT
                            break;
                        
                        case 0x81:      // EP1 IN
                            break;

                        case 0x02:      // EP2 OUT
                            break;
                        
                        case 0x82:      // EP2 IN
                            break;

                        case 0x03:      // EP3 OUT
                            break;
                        
                        case 0x83:      // EP3 IN
                            break;

                        default:
                            ControlState = ControlState | 0x80;
                            break;
                    }
                }
                else ControlState = ControlState | 0x80;
                break;

            case SET_FEATURE:
                if (DevRequest.bmRequestType.bits.recipient == USB_REQ_RECIP_DEVICE)
                {
                    if ((DevRequest.wValue & 0xFF) == 0x01)
                    {
                        if (ConfigDesc[7] & 0x20)
                        {
                            // wake up
                        }
                        else ControlState = ControlState | 0x80;
                    }
                    else ControlState = ControlState | 0x80;
                }
                else if (DevRequest.bmRequestType.bits.recipient == USB_REQ_RECIP_ENDP)
                {
                    switch (DevRequest.wIndex & 0xFF)
                    {
                        case 0x01:      // EP1 OUT
                            break;
                        
                        case 0x81:      // EP1 IN
                            break;

                        case 0x02:      // EP2 OUT
                            break;
                        
                        case 0x82:      // EP2 IN
                            break;

                        case 0x03:      // EP3 OUT
                            break;
                        
                        case 0x83:      // EP3 IN
                            break;

                        default:
                            ControlState = ControlState | 0x80;
                            break;
                    }
                }
                else ControlState = ControlState | 0x80;
                break;

            case GET_INTERFACE:
                break;
            
            case SET_INTERFACE:
                break;

            case GET_STATUS:
                EP0_buffer[0]   = 0x00;
                EP0_buffer[1]   = 0x00;
                bCount          = 2;
                buffin          = EP0_buffer;
                break;
            
            case SET_DESCRIPTOR:
                break;

            default:
                break;
        }
    }

    if ((ControlState & 0x80) == 0x80)     // Stall
    {
        ControlState = ControlState & ~0x80;
        USB_SET_STAT_TX(USBx, 0, STATUS_TX_STALL);
        USB_SET_STAT_RX(USBx, 0, STATUS_RX_STALL);
        return;
    }

    if ((ControlState & 0x02) != 0x02)      // Next IN Direction
    {
        bufftmp = buffin;

        if ((ControlState & 0x01) != 0x01)      // Next Data Stage
        {
            if (bCount > 0x40)
            {
                USB_COUNT0_TX   = 0x40;
                bCount          = bCount - 0x40;
                buffin          = buffin + 0x40;
                ControlState    = ControlState | 0x40;
            }
            else
            {
                USB_COUNT0_TX = bCount;
                bCount = 0;
                buffin = NULL;
            }
            
            USB_WritePMA(USBx, USB_ADDR0_TX, bufftmp, USB_COUNT0_TX & 0x3FF);
        }

        // Phải modify cả thanh ghi chứ từng bit riêng lẻ sẽ không đúng với các bit toggle
        // Cho phép transmit, DTOG_TX = 1 => Truyền DATA1
        // Vì Status Stage là DATA1 => DTOG_RX = 1 để cho phép nhận DATA1
        
        USB_SET_STAT_TX(USBx, 0, STATUS_TX_VALID);
        USB_DATA_TGL_TX(USBx, 0, DATA_TGL_1);
    }
    else                                   // Next OUT Direction
    {
        USB_SET_STAT_RX(USBx, 0, STATUS_RX_VALID);
        USB_DATA_TGL_RX(USBx, 0, DATA_TGL_1);
    }
}

static void USB_ProcessDataInStage(USB_Typedef* USBx, uint8_t ep)
{
    uint8_t     *bufftmp;

    if ((ControlState & 0x01) != 0x01)          /* Data Stage */
    {
        bufftmp         = buffin;

        if (bCount > 0x40)
        {
            USB_COUNT_TX(ep) = 0x40;
            bCount = bCount - 0x40;
            buffin = buffin + 0x40;
            USB_SET_STAT_RX(USBx, ep, STATUS_RX_NAK);
            USB_SET_STAT_TX(USBx, ep, STATUS_TX_VALID);
        }
        else
        {
            USB_COUNT_TX(ep) = bCount;
            bCount = 0;
            buffin = NULL;

            if ((ControlState & 0x40) == 0x40)
            {
                ControlState = ControlState & ~0x40;
                USB_SET_STAT_RX(USBx, ep, STATUS_RX_NAK);
                USB_SET_STAT_TX(USBx, ep, STATUS_TX_VALID);
            }
            else
            {
                ControlState    = ControlState | 0x03;  /* Next Status Stage and OUT direction */

                USB_SET_STAT_RX(USBx, ep, STATUS_RX_VALID);
                USB_SET_STAT_TX(USBx, ep, STATUS_TX_NAK);
                USB_DATA_TGL_RX(USBx, ep, DATA_TGL_1);
            }
        }

        USB_WritePMA(USBx, USB_ADDR_TX(ep), bufftmp, USB_COUNT_TX(ep) & 0x3FF);
    }
    else                                        /* Status Stage */
    {
        ControlState = 0;

        USB_SET_STAT_RX(USBx, ep, STATUS_RX_VALID);
        USB_SET_STAT_TX(USBx, ep, STATUS_TX_NAK);
        USB_DATA_TGL_RX(USBx, ep, DATA_TGL_0);

        if (u8Address > 0U && bCount == 0) 
        {
            USBx->DADDR.WORD = u8Address | 0x80;
            u8Address = 0;
        }
    }
}

static void USB_ProcessDataOutStage(USB_Typedef* USBx, uint8_t ep)
{
    register uint8_t  i;
    uint16_t wLength;
    
    if (ep == 0)                    // Control transfer
    {
        if ((ControlState & 0x01) != 0x01)          /* Data Stage */
        {
            ControlState = ControlState | 0x01;     /* Next Status Stage */
            
            USB_ReadPMA(USBx, USB_ADDR_RX(ep), buffout, USB_COUNT_RX(ep) & 0x3FF);

            if (ep == 0 && DevRequest.bRequest == CDC_SET_LINE_CODING)
            {
                wLength = DevRequest.wLength > sizeof(CDC_LineCoding) ? sizeof(CDC_LineCoding) : DevRequest.wLength;
                
                for (i = 0; i < wLength; ++i)
                {
                    ((uint8_t*)&CDC_LineCoding)[i] = buffout[i];
                }
            }

            USB_SET_STAT_TX(USBx, ep, STATUS_TX_VALID);
            USB_DATA_TGL_TX(USBx, ep, DATA_TGL_1);
            USB_SET_STAT_RX(USBx, ep, STATUS_RX_NAK);
        }
        else                                        /* Status Stage */
        {
            ControlState = 0;

            USB_SET_STAT_RX(USBx, ep, STATUS_RX_VALID);
            USB_DATA_TGL_RX(USBx, ep, DATA_TGL_0);
            USB_SET_STAT_TX(USBx, ep, STATUS_TX_NAK);
        }
    }
    else
    {
        if ((ControlState & 0x04) != 0x04)
        {
            ControlState = ControlState | 0x04;
            USB_ReadPMA(USBx, USB_ADDR_RX(ep), buffout, USB_COUNT_RX(ep) & 0x3FF);
            SignalTest(5);
        }
    }
}

void USB_PowerOnReset(void)
{
    /* Peripheral clock enable */
    RCC->APB1ENR.BITS.USBEN = SET;
    
    /*Enable interrupt NVIC */
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0X01);

    /* Enable interrupt mask */
    USB->CNTR.WORD = 0xBF00;
    
    /* Force reset */
    USB->CNTR.BITS.FRES = 0x01;
    USB->CNTR.BITS.FRES = 0x00;
    USB->ISTR.BITS.RESET = 0x00;

    /*Clear pending interrupts*/
    USB->ISTR.WORD = 0;

    /*Set Btable Address*/
    USB->BTABLE = BTABLE_ADDRESS;

    /* Enable interrupt mask */
    USB->CNTR.WORD = 0xBF00;
}

void USB_ResetCallBack(void)
{
    USB->ISTR.BITS.RESET = 0x00;

    USB_EndpointInit(USB, ENDPOINT_TYPE_CONTROL, 0x80, 0x18, USB_MAX_EP_PACKET_SIZE);
    USB_EndpointInit(USB, ENDPOINT_TYPE_CONTROL, 0x00, 0x58, USB_MAX_EP_PACKET_SIZE);

    USB->DADDR.BITS.EF = 0x01;
    USB->DADDR.BITS.ADD = 0x00;
}

void USB_TransactionCallBack(void)
{
    uint8_t nCounter    = 0;
    uint16_t addr       = 0;

    while (USB->ISTR.BITS.CTR != RESET)
    {
        epindex = USB->ISTR.BITS.EP_ID;
        
        /* Endpoint 0 */
        if (epindex == 0)
        {
            /* DIR bit = Direction of transaction */

            /* DIR = 1 => Out type => CTR_RX bit or both CTR_TX/CTR_RX are set*/   
            if (USB->ISTR.BITS.DIR != RESET)
            {
                // Có khả năng lỗi sẽ phát sinh tại đây, vì tại đây mình đang fix cứng
                // là cả OUT/SETUP token chứ không xét từng trường hợp OUT và SETUP token
                // => Vào case OUT token có thể fail
                // => Lúc đấy có thể check tiếp hoặc thôi :v

                if (USB->EPnRp[epindex].BITS.SETUP != RESET)
                {
                    // Chỉ có thể đọc Reception byte count này khi hoàn thành
                    // OUT/SETUP transaction
                    nCounter = USB_COUNT0_RX & 0x3FF;
                    addr     = USB_ADDR0_RX;

                    USB_ReadPMA(USB, addr, buffout, nCounter);

                    CLEAR_TRANSFER_RX_FLAG(USB, epindex);
                    
                    USB_ProcessSetupStage(USB, buffout);
                }

                // Đã fix được, do Status Stage nó là Out Token để host gửi ACK cho device
                // mà trước đó mình để code của Out Token rỗng => Sẽ gây ra sai sót liên
                // quan đến STAT_RX và DTOG_RX => Fail
                else
                {
                    if (USB->EPnRp[epindex].BITS.CTR_RX != RESET)
                    {
                        CLEAR_TRANSFER_RX_FLAG(USB, epindex);
                        USB_ProcessDataOutStage(USB, epindex);
                    }
                }
            }
            /* DIR = 0 => IN type => CTR_TX bit is set */
            else
            { 
                // In token
                if (USB->EPnRp[epindex].BITS.CTR_TX != RESET)
                {
                    // Clear bit CTR_TX
                    CLEAR_TRANSFER_TX_FLAG(USB, epindex);
                    USB_ProcessDataInStage(USB, epindex);
                }
            }
        }
        else
        {
            /* DIR bit = Direction of transaction */

            /* DIR = 1 => Out type => CTR_RX bit or both CTR_TX/CTR_RX are set*/   
            if (USB->ISTR.BITS.DIR != RESET)
            {
                // Out token
                if (USB->EPnRp[epindex].BITS.CTR_RX != RESET)
                {
                    CLEAR_TRANSFER_RX_FLAG(USB, epindex);                 
                    USB_ProcessDataOutStage(USB, epindex);
                }
            }
            /* DIR = 0 => IN type => CTR_TX bit is set */
            else
            {
                // In token

                if (USB->EPnRp[epindex].BITS.CTR_TX != RESET)
                {
                    // Clear bit CTR_TX
                    CLEAR_TRANSFER_TX_FLAG(USB, epindex);
                    USB_ProcessDataInStage(USB, epindex);
                }
            }
        }
    }
}

void CDC_Transmit(uint8_t* data)
{
    uint8_t* bufftmp = NULL;
    
    if (!USB_ENUM_OK) return;

    buffin = bufftmp = data;
    bCount = USBD_GetLen(data);

    if (bCount > 0x40)
    {
        USB_COUNT_TX(1)   = 0x40;
        bCount          = bCount - 0x40;
        buffin          = buffin + 0x40;
        ControlState    = ControlState | 0x40;
    }
    else
    {
        USB_COUNT_TX(1) = bCount;
        bCount = 0;
        buffin = NULL;
    }

    USB_WritePMA(USB, USB_ADDR_TX(1), bufftmp, USB_COUNT_TX(1) & 0x3FF);
    USB_SET_STAT_TX(USB, 0x01, STATUS_TX_VALID);
}

uint16_t CDC_Receive(uint8_t** data)
{
    uint16_t bCount = 0;

    if ((ControlState & 0x04) == 0x04)
    {
        *data = buffout;
        ControlState = ControlState & ~0x04;
        bCount = USB_COUNT_RX(0x01) & 0x3FF;
        buffout[bCount] = '\0';
        USB_SET_STAT_RX(USB, 0x01, STATUS_RX_VALID);
    }

    return bCount;
}
