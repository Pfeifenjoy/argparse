#ifndef VECTOR_H_UXBEAEUM
#define VECTOR_H_UXBEAEUM

#include "stdlib.h"

struct s_vector {
	char **data;
	size_t length;
};

void s_vector_init(struct s_vector *);

void s_vector_add(struct s_vector *, const char *);

void s_vector_destroy(struct s_vector *);

#endif /* end of include guard: VECTOR_H_UXBEAEUM */
