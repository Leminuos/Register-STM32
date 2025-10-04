#include "net_utils.h"

uint32_t net_crc32(const uint8_t* data, uint16_t len)
{
    uint32_t crc = 0xFFFFFFFF;

    for (uint16_t i = 0; i < len; i++) {
        uint8_t byte = data[i];
        crc ^= (uint32_t)byte;

        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;  // Reflected poly of 0x04C11DB7
            else
                crc >>= 1;
        }
    }

    return ~crc;
}

uint16_t net_checksum16(const void *data, uint16_t len)
{
    const uint8_t *p = (const uint8_t *)data;
    uint32_t sum = 0;

    while (len > 1)
    {
        uint16_t w = ((uint16_t)p[0] << 8) | (uint16_t)p[1];
        sum += w;
        p += 2;
        len -= 2;
    }

    if (len == 1)
    {
        uint16_t w = ((uint16_t)p[0] << 8);
        sum += w;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (uint16_t)(~sum);
}
