#ifndef MEMORY_H_08PDNX7V
#define MEMORY_H_08PDNX7V

#include "stdlib.h"

/**
 * Allocate memory and print error message.
 */
void *allocate(size_t amount, size_t size);

void *reallocate(void *, size_t amount, size_t size);

void *copy(const void *, size_t);

#endif /* end of include guard: MEMORY_H_08PDNX7V */
