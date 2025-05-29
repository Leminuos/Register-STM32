#include "tlsf.h"
#include "mem.h"

static uint8_t heap_area[HEAP_SIZE];
static tlsf_t tlsf;

void mem_init(void)
{
    tlsf = tlsf_create_with_pool(heap_area, HEAP_SIZE);

    if (tlsf == NULL)
    {
        printf("coundn't init heap\r\n");
    }
}

void* malloc(size_t size)
{
    printf("allocating %lu bytes\r\n", (unsigned long)size);
    if(size == 0) {
        printf("using zero_mem\r\n");
        return NULL;
    }

    void* alloc = tlsf_malloc(tlsf, size);

    if(alloc == NULL) {
        printf("couldn't allocate memory (%lu bytes)\r\n", (unsigned long)size);
        return NULL;
    }

    printf("allocated at %p\r\n", alloc);
    return alloc;
}

void* malloc_zeroed(size_t size)
{
    size_t i = 0;

    printf("allocating %lu bytes\r\n", (unsigned long)size);
    if(size == 0) {
        printf("using zero_mem\r\n");
        return NULL;
    }

    void* alloc = tlsf_malloc(tlsf, size);
    if(alloc == NULL) {
        printf("couldn't allocate memory (%lu bytes)\r\n", (unsigned long)size);
        return NULL;
    }

    for (i = 0; i < size; ++i) ((uint8_t*)alloc)[i] = 0;

    printf("allocated at %p\r\n", alloc);
    return alloc;
}

void* calloc(size_t num, size_t size)
{
    printf("allocating number of %zu each %zu bytes\r\n", num, size);
    return malloc_zeroed(num * size);
}

void* zalloc(size_t size)
{
    return malloc_zeroed(size);
}

void free(void* data)
{
    printf("freeing %p\r\n", data);
    if(data == NULL) return;
    if(data == NULL) return;

    tlsf_free(tlsf, data);
}

void* realloc(void* data_p, size_t new_size)
{
    printf("reallocating %p with %lu size\r\n", data_p, (unsigned long)new_size);
    if(new_size == 0) {
        printf("using zero_mem\r\n");
        free(data_p);
        return NULL;
    }

    if(data_p == NULL) return malloc(new_size);

    void* new_p = tlsf_realloc(tlsf, data_p, new_size);

    if(new_p == NULL) {
        printf("couldn't reallocate memory\r\n");
        return NULL;
    }

    printf("reallocated at %p\r\n", new_p);
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
