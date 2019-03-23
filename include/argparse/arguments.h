#ifndef ARGUMENTS_H_XUL1CTIC
#define ARGUMENTS_H_XUL1CTIC

#include "argparse/argument.h"
#include "argparse/vector.h"

typedef generic_vector_t arguments_t;

void arguments_init(arguments_t *);
void arguments_add(arguments_t *, argument_t *);
argument_t *arguments_get(arguments_t *, size_t);
const argument_t *arguments_get_const(const arguments_t *, size_t);
bool arguments_equal(const arguments_t *, const arguments_t *);
void arguments_destroy(arguments_t *);

#endif /* end of include guard: ARGUMENTS_H_XUL1CTIC */
