#ifndef OPTIONAL_H_Q9N3AOUN
#define OPTIONAL_H_Q9N3AOUN

#include "argparse/vector.h"
#include "argparse/arguments.h"
#include "argparse/context.h"

#include "stdlib.h"

typedef void(*optional_set_t)(const context_t *);

typedef struct {
	char short_name;
	const char *long_name;
	const char *description;
	optional_set_t set;
	arguments_t arguments;
	arguments_t optional_arguments;
} optional_t;

void optional_init(optional_t *);

void optional_add_argument(optional_t *, argument_t);
void optional_add_optional_argument(optional_t *, argument_t);

void optional_destroy(optional_t *);

#endif /* end of include guard: OPTIONAL_H_Q9N3AOUN */
