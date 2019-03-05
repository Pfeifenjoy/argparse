#ifndef ARGUMENTS_H_XUL1CTIC
#define ARGUMENTS_H_XUL1CTIC

#include "argparse/argument.h"

#include "stdlib.h"

struct arguments {
	struct argument *data;
	size_t size;
};

void arguments_init(struct arguments *);
void arguments_add(struct arguments *, struct argument);
void arguments_destroy(struct arguments *);

#endif /* end of include guard: ARGUMENTS_H_XUL1CTIC */
