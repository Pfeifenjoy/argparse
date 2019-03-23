#ifndef ARGUMENTS_H_XUL1CTIC
#define ARGUMENTS_H_XUL1CTIC

#include "argparse/argument.h"

#include "stdlib.h"

typedef struct {
	argument_t *data;
	size_t size;
} arguments_t;

void arguments_init(arguments_t *);
void arguments_add(arguments_t *, argument_t);
bool arguments_equal(const arguments_t *, const arguments_t *);
void arguments_destroy(arguments_t *);

#endif /* end of include guard: ARGUMENTS_H_XUL1CTIC */
