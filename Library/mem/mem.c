#include "tlsf.h"
#include "mem.h"

#if USE_TLSF_DEBUG
    #include "debug.h"
    #define TLSF_LOG_TRACE(format, ...)     TLSF_LOG_TRACE(format, ##__VA_ARGS__)
#else
    #define TLSF_LOG_TRACE(format, ...)
#endif /* USE_TLSF_DEBUG */

static uint8_t heap_area[HEAP_SIZE];
static tlsf_t tlsf;

void mem_init(void)
{
    tlsf = tlsf_create_with_pool(heap_area, HEAP_SIZE);

    if (tlsf == NULL)
    {
        TLSF_LOG_TRACE("coundn't init heap\r\n");
    }
}

void* malloc(size_t size)
{
    TLSF_LOG_TRACE("allocating %d bytes\r\n", (unsigned long)size);
    if(size == 0) {
        TLSF_LOG_TRACE("using zero_mem\r\n");
        return NULL;
    }

    void* alloc = tlsf_malloc(tlsf, size);

    if(alloc == NULL) {
        TLSF_LOG_TRACE("couldn't allocate memory (%d bytes)\r\n", (unsigned long)size);
        return NULL;
    }

    TLSF_LOG_TRACE("allocated at 0x%08X\r\n", alloc);
    return alloc;
}

void* malloc_zeroed(size_t size)
{
    size_t i = 0;

    TLSF_LOG_TRACE("allocating %d bytes\r\n", (unsigned long)size);
    if(size == 0) {
        TLSF_LOG_TRACE("using zero_mem\r\n");
        return NULL;
    }

    void* alloc = tlsf_malloc(tlsf, size);
    if(alloc == NULL) {
        TLSF_LOG_TRACE("couldn't allocate memory (%d bytes)\r\n", (unsigned long)size);
        return NULL;
    }

    for (i = 0; i < size; ++i) ((uint8_t*)alloc)[i] = 0;

    TLSF_LOG_TRACE("allocated at 0x%08X\r\n", alloc);
    return alloc;
}

void* calloc(size_t num, size_t size)
{
    TLSF_LOG_TRACE("allocating number of %d each %d bytes\r\n", num, size);
    return malloc_zeroed(num * size);
}

void* zalloc(size_t size)
{
    return malloc_zeroed(size);
}

void free(void* data)
{
    TLSF_LOG_TRACE("freeing 0x%08X\r\n", data);
    if(data == NULL) return;
    if(data == NULL) return;

    tlsf_free(tlsf, data);
}

void* realloc(void* data_p, size_t new_size)
{
    TLSF_LOG_TRACE("reallocating 0x%08X with %d size\r\n", data_p, (unsigned long)new_size);
    if(new_size == 0) {
        TLSF_LOG_TRACE("using zero_mem\r\n");
        free(data_p);
        return NULL;
    }

    if(data_p == NULL) return malloc(new_size);

    void* new_p = tlsf_realloc(tlsf, data_p, new_size);

    if(new_p == NULL) {
        TLSF_LOG_TRACE("couldn't reallocate memory\r\n");
        return NULL;
    }

    TLSF_LOG_TRACE("reallocated at 0x%08X\r\n", new_p);
    return new_p;
}

void* reallocf(void* data_p, size_t new_size)
{
    void* new = realloc(data_p, new_size);
    
    if(!new) {
        free(data_p);
    }
    return new;
}
