#ifndef VECTOR_H_UXBEAEUM
#define VECTOR_H_UXBEAEUM

#include "stdlib.h"

typedef struct {
	char **data;
	size_t length;
} s_vector_t;

void s_vector_init(s_vector_t *);

void s_vector_add(s_vector_t *, const char *);

void s_vector_destroy(s_vector_t *);

#endif /* end of include guard: VECTOR_H_UXBEAEUM */
