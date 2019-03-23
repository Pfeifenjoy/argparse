#ifndef VECTOR_H_UXBEAEUM
#define VECTOR_H_UXBEAEUM

#include "stdlib.h"

typedef struct {
	void **blocks;
	size_t length;
	size_t element_size;
} generic_vector_t;

void generic_vector_init(generic_vector_t *, size_t);

void *generic_vector_add(generic_vector_t *, const void *);

void *generic_vector_get(generic_vector_t *, size_t);
const void *generic_vector_get_const(const generic_vector_t *, size_t);

void generic_vector_destroy(generic_vector_t *);

typedef generic_vector_t s_vector_t;

void s_vector_init(s_vector_t *);

void s_vector_add(s_vector_t *, const char *);

char *s_vector_get(s_vector_t *, size_t);
const char *s_vector_get_const(const s_vector_t *, size_t);

void s_vector_destroy(s_vector_t *);

#endif /* end of include guard: VECTOR_H_UXBEAEUM */
