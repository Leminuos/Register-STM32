#ifndef __NET_UTILS_H__
#define __NET_UTILS_H__

#include <stdint.h>

#if defined(__GNUC__)
  #define PACKED __attribute__((packed))
#else
  #pragma pack(push, 1)
  #define PACKED
#endif

static inline uint16_t htons(uint16_t x) { return (x<<8) | (x>>8); }

uint32_t net_crc32(const uint8_t* data, uint16_t len);
uint16_t net_checksum16(const void *data, uint16_t len);

#endif /* __NET_UTILS_H__ */
