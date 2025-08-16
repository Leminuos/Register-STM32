#include "uart_ring_buffer.h"
#include "stm32_driver_uart.h"

ring_buffer_t r;

uint8_t ring_buffer_empty(void)
{
    return r.pread == r.pwrite;
}

uint8_t ring_buffer_full(void)
{
    return ((r.pwrite + 1) % RING_BUFFER_SIZE) == r.pread;
}

uint8_t ring_buffer_put(char c)
{
    if (ring_buffer_full() != 0) return STATE_RING_BUFFER_FULL;

    r.buffer[r.pwrite] = c;
    r.pwrite = (++r.pwrite) % RING_BUFFER_SIZE;

    return STATE_RING_BUFFER_SUCCESS;
}

uint8_t ring_buffer_get(char* c)
{
    if (ring_buffer_empty() != 0) return STATE_RING_BUFFER_EMPTY;

    *c = r.buffer[r.pread];
    r.pread = (++r.pread) % RING_BUFFER_SIZE;

    return STATE_RING_BUFFER_SUCCESS;
}

void ring_buffer_dump(void)
{
    char c = 0;

    while (ring_buffer_get(&c) == STATE_RING_BUFFER_SUCCESS)
        USART_Transmiter(USART1, c);
}

