#include "tlsf.h"
#include "mem.h"
#include "debug.h"

static uint8_t heap_area[HEAP_SIZE];
static tlsf_t tlsf;

void mem_init(void)
{
    tlsf = tlsf_create_with_pool(heap_area, HEAP_SIZE);

    if (tlsf == NULL)
    {
        debug_print("coundn't init heap\r\n");
    }
}

void* malloc(size_t size)
{
    debug_print("allocating %lu bytes\r\n", (unsigned long)size);
    if(size == 0) {
        debug_print("using zero_mem\r\n");
        return NULL;
    }

    void* alloc = tlsf_malloc(tlsf, size);

    if(alloc == NULL) {
        debug_print("couldn't allocate memory (%lu bytes)\r\n", (unsigned long)size);
        return NULL;
    }

    debug_print("allocated at %p\r\n", alloc);
    return alloc;
}

void* malloc_zeroed(size_t size)
{
    size_t i = 0;

    debug_print("allocating %lu bytes\r\n", (unsigned long)size);
    if(size == 0) {
        debug_print("using zero_mem\r\n");
        return NULL;
    }

    void* alloc = tlsf_malloc(tlsf, size);
    if(alloc == NULL) {
        debug_print("couldn't allocate memory (%lu bytes)\r\n", (unsigned long)size);
        return NULL;
    }

    for (i = 0; i < size; ++i) ((uint8_t*)alloc)[i] = 0;

    debug_print("allocated at %p\r\n", alloc);
    return alloc;
}

void* calloc(size_t num, size_t size)
{
    debug_print("allocating number of %zu each %zu bytes\r\n", num, size);
    return malloc_zeroed(num * size);
}

void* zalloc(size_t size)
{
    return malloc_zeroed(size);
}

void free(void* data)
{
    debug_print("freeing %p\r\n", data);
    if(data == NULL) return;
    if(data == NULL) return;

    tlsf_free(tlsf, data);
}

void* realloc(void* data_p, size_t new_size)
{
    debug_print("reallocating %p with %lu size\r\n", data_p, (unsigned long)new_size);
    if(new_size == 0) {
        debug_print("using zero_mem\r\n");
        free(data_p);
        return NULL;
    }

    if(data_p == NULL) return malloc(new_size);

    void* new_p = tlsf_realloc(tlsf, data_p, new_size);

    if(new_p == NULL) {
        debug_print("couldn't reallocate memory\r\n");
        return NULL;
    }

    debug_print("reallocated at %p\r\n", new_p);
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
