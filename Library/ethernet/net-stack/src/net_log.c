#include "net_log.h"

const char* net_log_err(net_state_t err)
{
    switch (err) {
        case NET_OK: return "No error";
        case NET_ERR_TIMEOUT: return "Operation timed out";
        case NET_ERR_DRV_FAILED_INIT: return "Driver failed to initialize";
        case NET_ERR_NULL_HANDLE: return "Null handle passed";
        case NET_ERR_PARAM_INVALID: return "Invalid parameter";
        case NET_ERR_FORMAT_INVALID: return "Invalid frame/packet format";
        case NET_ERR_INVALID_IP: return "Invalid IP address";
        case NET_ERR_INVALID_MAC: return "Invalid MAC address";
        case NET_ERR_VALUE_OUT_OF_RANGE: return "Value out of range";
        case NET_ERR_TX_DRIVER_FAIL: return "Transmit driver failure";
        case NET_ERR_RX_DRIVER_FAIL: return "Receive driver failure";
        case NET_ERR_FRAME_TOO_SHORT: return "Frame too short";
        case NET_ERR_FRAME_TOO_LONG: return "Frame too long";
        case NET_ERR_UNSUPPORTED_ETHER_TYPE: return "Unsupported EtherType";
        case NET_ERR_DST_NOT_FOR_ME: return "Frame not for this MAC";
        case NET_ERR_HANDLER_TABLE_FULL: return "Handler table full";
        case NET_ERR_NO_HANDLER_FOUND: return "No handler found for EtherType";
        case NET_ERR_GET_ARP_TABLE: return "Failed to get ARP table entry";
        case NET_WARN_PADDING_ADDED: return "Padding added";
        case NET_WARN_MAC_BROADCAST: return "Broadcast MAC";
        default: return "Unknown error";
    }
}
