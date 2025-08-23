#ifndef __UART_RING_BUFFER__
#define __UART_RING_BUFFER__

#include <stdint.h>

#define STATE_RING_BUFFER_SUCCESS   0x00
#define STATE_RING_BUFFER_FULL      0x01
#define STATE_RING_BUFFER_EMPTY     0x02


#define RING_BUFFER_SIZE            0x400 

typedef struct {
    uint32_t    pread;
    uint32_t    pwrite;
    char        buffer[RING_BUFFER_SIZE];
} ring_buffer_t;

extern void ring_buffer_dump(void);
extern uint8_t ring_buffer_empty(void);
extern uint8_t ring_buffer_full(void);
extern uint8_t ring_buffer_put(char c);
extern uint8_t ring_buffer_get(char* c);

#endif  /* __UART_RING_BUFFER__ */

