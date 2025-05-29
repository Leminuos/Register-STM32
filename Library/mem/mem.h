#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>
#include <stdio.h>

#define HEAP_SIZE  4096  // 4KB heap

void mem_init(void);
void free(void* data);
void* malloc(size_t size);
void* malloc_zeroed(size_t size);
void* calloc(size_t num, size_t size);
void* zalloc(size_t size);
void* realloc(void* data_p, size_t new_size);
void* reallocf(void* data_p, size_t new_size);

#endif /* _MEM_H_ */

