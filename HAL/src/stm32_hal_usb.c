#include <string.h>
#include "stm32_hal_usb.h"

#if ENABLE_DEBUG_USB == 1
static const char* TAG = "USB";
#endif /* ENABLE_DEBUG_USB */

extern __ALIGN_BEGIN uint8_t DeviceDesc[MAX_SIZE_DEVICE_DESCRIPTOR] __ALIGN_END;
extern __ALIGN_BEGIN uint8_t ConfigDesc[USB_CONFIG_DESC_LEN] __ALIGN_END;
extern __ALIGN_BEGIN uint8_t StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

static USB_RequestTypedef   DevRequest;
static uint8_t*             in_buffer;
static uint8_t              out_buffer[USB_MAX_EP0_SIZE];
static uint8_t              epindex;
static uint8_t              usb_address;
static uint8_t              usb_config;
static uint8_t              usb_data_toggle;
static uint16_t             usb_maxlen;
uint8_t usb_enum_flag;

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

static void USB_GetString(uint8_t *desc, uint8_t *unicode, uint8_t *len)
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
            bCount = bCount << 10;
            bCount = bCount | 0x8000;   // Set BL_SIZE
        }
        else
        {
            bCount = (count + 1U) >> 1U;            
            bCount = bCount << 10;
        }

        USB_ADDR_RX(ep) = addr;
        USB_COUNT_RX(ep) = bCount;
    }
    
    return 0;
}

static void USB_SetupTransaction(USB_Typedef* USBx, uint8_t *buff)
{
    uint8_t  len = 0;
    uint8_t  setup_buffer[10];

    usb_data_toggle = DATA_TGL_1;
    DevRequest = *((USB_RequestTypedef*) buff);
    usb_maxlen = DevRequest.wLength;

    if (DevRequest.bmRequestType.bits.type == USB_REQ_TYP_STANDARD)
    {
        switch (DevRequest.bRequest)
        {
            case GET_DESCRIPTOR:
                switch (DevRequest.wValueH)
                {
                    case DEVICE_TYPE:
                        len = MAX_SIZE_DEVICE_DESCRIPTOR;
                        in_buffer = DeviceDesc;
                        break;

                    case CONFIGURATION_TYPE:
                        len = USB_CONFIG_DESC_LEN;
                        in_buffer = ConfigDesc;
                        break;

                    case STRING_TYPE:
                        // USB Language Identifiers
                        // http://www.baiheee.com/Documents/090518/090518112619/USB_LANGIDs.pdf

                        switch (DevRequest.wValueL)
                        {
                            case USBD_IDX_LANGID_STR:
                                USB_GetString((uint8_t *)USBD_LANGID_STRING, StrDesc, &len);
                                break;

                            case USBD_IDX_MFC_STR:
                                USB_GetString((uint8_t *)USBD_MANUFACTURER_STRING, StrDesc, &len);
                                break;

                            case USBD_IDX_PRODUCT_STR:
                                USB_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, StrDesc, &len);
                                break;

                            case USBD_IDX_SERIAL_STR:
                                USB_GetString((uint8_t *)USBD_SERIALNUMBER_STRING_FS, StrDesc, &len);
                                break;

                            case USBD_IDX_CONFIG_STR:
                                USB_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, StrDesc, &len);
                                break;

                            case USBD_IDX_INTERFACE_STR:
                                USB_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, StrDesc, &len);
                                break;

                            default:
                                USB_GetString((uint8_t *)USBD_SERIALNUMBER_STRING_FS, StrDesc, &len);
                                break;
                        }

                        in_buffer = StrDesc;
                        break;
                    
                    #if SUPPORT_USB_HID
                    case REPORT_DESCRIPTOR_TYPE:
                        len = DevRequest.wLength;
                        in_buffer = reportDescriptor;
                        break;
                    #endif /* SUPPORT_USB_HID */

                    default:
                        len = 0xFF;
                        break;
                }

                break;
            
            case SET_ADDRESS:
                usb_address  = DevRequest.wValueL;
                DEBUG_USB(LOG_VERBOSE, TAG, "SET_ADDRESS: 0x%02X", usb_address);
                break;

            case GET_CONFIGURATION:
                setup_buffer[0] = usb_config;
                in_buffer      = setup_buffer;
                len             = 1;
                break;
            
            case SET_CONFIGURATION:
                usb_config      = DevRequest.wValueL;
            #ifdef USB_CLASS_ENDPOINT_INIT
                USB_CLASS_ENDPOINT_INIT();
            #endif /* USB_CLASS_ENDPOINT_INIT */
                usb_enum_flag   = 1;
                break;

            case CLEAR_FEATURE:
                if (DevRequest.bmRequestType.bits.recipient == USB_REQ_RECIP_DEVICE)
                {
                    if (DevRequest.wValueL == 0x01)
                    {
                        if (ConfigDesc[7] & 0x20)
                        {
                            // wake up
                        }
                        else len = 0xFF;
                    }
                    else len = 0xFF;
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
                            len = 0xFF;
                            break;
                    }
                }
                else len = 0xFF;
                break;

            case SET_FEATURE:
                if (DevRequest.bmRequestType.bits.recipient == USB_REQ_RECIP_DEVICE)
                {
                    if (DevRequest.wValueL == 0x01)
                    {
                        if (ConfigDesc[7] & 0x20)
                        {
                            // wake up
                        }
                        else len = 0xFF;
                    }
                    else len = 0xFF;
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
                            len = 0xFF;
                            break;
                    }
                }
                else len = 0xFF;
                break;

            case GET_INTERFACE:
                break;
            
            case SET_INTERFACE:
                break;

            case GET_STATUS:
                setup_buffer[0] = 0x00;
                setup_buffer[1] = 0x00;
                len             = 2;
                in_buffer       = setup_buffer;
                break;
            
            case SET_DESCRIPTOR:
                break;

            default:
                break;
        }
    }

#ifdef USB_CLASS_SETUP_CALLBACK
    else if (DevRequest.bmRequestType.bits.type == USB_REQ_TYP_CLASS)
    {
        len = USB_CLASS_SETUP_CALLBACK(DevRequest.bRequest, &in_buffer);
    }
#endif /* USB_CLASS_SETUP_CALLBACK */

#ifdef USB_VENDOR_SETUP_CALLBACK
    else if (DevRequest.bmRequestType.bits.type == USB_REQ_TYP_VENDOR)
    {
        USB_VENDOR_SETUP_CALLBACK(DevRequest.bRequest, &in_buffer);
    }
#endif /* USB_VENDOR_SETUP_CALLBACK */

    else len = 0xFF;

    if (len == 0xFF)     // Stall
    {
        USB_SET_STAT_TX(USBx, 0, STATUS_TX_STALL);
        USB_SET_STAT_RX(USBx, 0, STATUS_RX_STALL);
        return;
    }

    usb_maxlen = len < DevRequest.wLength ? len : DevRequest.wLength;
    len = usb_maxlen > USB_MAX_EP_PACKET_SIZE ? USB_MAX_EP_PACKET_SIZE : usb_maxlen;
    USB_COUNT0_TX = len;
    USB_WritePMA(USBx, USB_ADDR0_TX, in_buffer, USB_COUNT0_TX & 0x3FF);
    in_buffer = in_buffer + len;
    usb_maxlen = usb_maxlen - len;

    // Phải modify cả thanh ghi chứ từng bit riêng lẻ sẽ không đúng với các bit toggle
    // Cho phép transmit, DTOG_TX = 1 => Truyền DATA1
    // Vì Status Stage là DATA1 => DTOG_RX = 1 để cho phép nhận DATA1
    
    USB_SET_STAT_TX(USBx, 0, STATUS_TX_VALID);
    USB_DATA_TGL_TX(USBx, 0, DATA_TGL_1);
    USB_SET_STAT_RX(USBx, 0, STATUS_RX_VALID);
    USB_DATA_TGL_RX(USBx, 0, DATA_TGL_1);
}

static void USB_EP0_IN_Transaction(USB_Typedef* USBx)
{
    uint8_t len = 0;

    switch (DevRequest.bRequest)
    {
        case GET_DESCRIPTOR:
            len = usb_maxlen > USB_MAX_EP_PACKET_SIZE ? USB_MAX_EP_PACKET_SIZE : usb_maxlen;

            if (len > 0)
            {
                USB_COUNT0_TX = len;
                USB_WritePMA(USBx, USB_ADDR0_TX, in_buffer, USB_COUNT0_TX & 0x3FF);
                usb_data_toggle = !usb_data_toggle;
                in_buffer = in_buffer + len;
                usb_maxlen = usb_maxlen - len;
                USB_SET_STAT_RX(USBx, 0x00, STATUS_RX_NAK);
                USB_SET_STAT_TX(USBx, 0x00, STATUS_TX_VALID);
                USB_DATA_TGL_TX(USBx, 0x00, usb_data_toggle);
            }
            else
            {
                USB_SET_STAT_RX(USBx, 0x00, STATUS_RX_VALID);
                USB_SET_STAT_TX(USBx, 0x00, STATUS_TX_NAK);
                USB_DATA_TGL_RX(USBx, 0x00, DATA_TGL_1);
            }

            break;

        case SET_ADDRESS:
            USBx->DADDR.WORD = usb_address | 0x80;
            usb_address = 0;
        
        default:
            USB_SET_STAT_RX(USBx, 0x00, STATUS_RX_VALID);
            USB_SET_STAT_TX(USBx, 0x00, STATUS_TX_NAK);
            USB_DATA_TGL_RX(USBx, 0x00, DATA_TGL_1);
            break;
    }
}

static void USB_EP0_OUT_Transaction(USB_Typedef* USBx)
{
    USB_ReadPMA(USBx, USB_ADDR_RX(0x00), out_buffer, USB_COUNT_RX(0x00) & 0x3FF);

#ifdef USB_EP0_OUT_HANDLER
    USB_EP0_OUT_HANDLER((void*)&DevRequest, out_buffer);
#endif /* USB_EP0_OUT_HANDLER */

    USB_SET_STAT_TX(USBx, 0x00, STATUS_TX_VALID);
    USB_DATA_TGL_TX(USBx, 0x00, DATA_TGL_1);
    USB_SET_STAT_RX(USBx, 0x00, STATUS_RX_VALID);
    USB_DATA_TGL_RX(USBx, 0x00, DATA_TGL_1);
}

// Read Packet Buffer Memory Address
void USB_ReadPMA(USB_Typedef* USBx , uint16_t wBufAddrPMA, uint8_t* buff, uint16_t wCount)
{
    uint32_t index          = 0;
    uint32_t nCount         = (wCount + 1) >> 1;
    uint32_t* pBufAddrAPB   = 0;

    pBufAddrAPB = (uint32_t*) (wBufAddrPMA*2 + (uint32_t) USBx + 0x400);

    for (index = 0; index < nCount; ++index)
    {
        *((uint16_t*) buff) = *((uint16_t*) pBufAddrAPB);
        pBufAddrAPB++;
        buff = buff + 2;
    }
}

// Write Packet Buffer Memory Address 
void USB_WritePMA(USB_Typedef* USBx, uint16_t wBufAddrPMA, uint8_t* buff, uint16_t wCount)
{
    uint32_t index          = 0;
    uint32_t nCount         = (wCount + 1) >> 1;
    uint16_t *pBufAddrAPB   = 0, temp1, temp2;

    pBufAddrAPB = (uint16_t*) (wBufAddrPMA*2 + (uint32_t) USBx + 0x400);

    if (buff == NULL || wCount == 0) return;

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

void USB_EndpointInit(USB_Typedef* USBx, uint8_t type, uint8_t addr, uint16_t packetAddr, uint16_t maxPacketSize)
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

void USB_PowerOnReset(void)
{
    /* Peripheral clock enable */
    RCC->APB1ENR.BITS.USBEN = SET;
    
    /*Enable interrupt NVIC */
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0x01);

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

    USB->DADDR.BITS.EF = 0x01;
    USB->DADDR.BITS.ADD = 0x00;

    USB_EndpointInit(USB, ENDPOINT_TYPE_CONTROL, 0x80, 0x18, USB_MAX_EP_PACKET_SIZE);
    USB_EndpointInit(USB, ENDPOINT_TYPE_CONTROL, 0x00, 0x58, USB_MAX_EP_PACKET_SIZE);

    DEBUG_USB(LOG_VERBOSE, TAG, "Host send reset signal");
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
                if (USB->EPnRp[epindex].BITS.SETUP != RESET)
                {
                    nCounter = USB_COUNT0_RX & 0x3FF;
                    addr     = USB_ADDR0_RX;

                    USB_ReadPMA(USB, addr, out_buffer, nCounter);

                    CLEAR_TRANSFER_RX_FLAG(USB, epindex);
                    
                    USB_SetupTransaction(USB, out_buffer);
                }
                else
                {
                    if (USB->EPnRp[epindex].BITS.CTR_RX != RESET)
                    {
                        CLEAR_TRANSFER_RX_FLAG(USB, epindex);
                        USB_EP0_OUT_Transaction(USB);
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
                    USB_EP0_IN_Transaction(USB);
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
                    
                    switch (epindex)
                    {
                        #ifdef USB_EP1_OUT_HANDLER
                        case 1: USB_EP1_OUT_HANDLER(); break;
                        #endif /* USB_EP1_OUT_HANDLER */

                        #ifdef USB_EP2_OUT_HANDLER
                        case 2: USB_EP2_OUT_HANDLER(); break;
                        #endif /* USB_EP2_OUT_HANDLER */

                        #ifdef USB_EP3_OUT_HANDLER
                        case 3: USB_EP3_OUT_HANDLER(); break;
                        #endif /* USB_EP3_OUT_HANDLER */

                        #ifdef USB_EP4_OUT_HANDLER
                        case 4: USB_EP4_OUT_HANDLER(); break;
                        #endif /* USB_EP4_OUT_HANDLER */

                        default: break;
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
                    
                    switch (epindex)
                    {
                        #ifdef USB_EP1_IN_HANDLER
                        case 1: USB_EP1_IN_HANDLER(); break;
                        #endif /* USB_EP1_OUT_HANDLER */

                        #ifdef USB_EP2_IN_HANDLER
                        case 2: USB_EP2_IN_HANDLER(); break;
                        #endif /* USB_EP2_IN_HANDLER */

                        #ifdef USB_EP3_IN_HANDLER
                        case 3: USB_EP3_IN_HANDLER(); break;
                        #endif /* USB_EP3_IN_HANDLER */

                        #ifdef USB_EP4_IN_HANDLER
                        case 4: USB_EP4_IN_HANDLER(); break;
                        #endif /* USB_EP4_IN_HANDLER */

                        default: break;
                    }
                }
            }
        }
    }
}

void USB_ResumeCallBack(void)
{
    /* Do something */ 
}

void USB_SuspendCallBack(void)
{
    /* Do something */ 
}

