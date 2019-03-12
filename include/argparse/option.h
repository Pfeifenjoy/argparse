#ifndef OPTION_H_Q9N3AOUN
#define OPTION_H_Q9N3AOUN

#include "argparse/vector.h"
#include "argparse/arguments.h"
#include "argparse/context.h"

#include "stdlib.h"

typedef void(*option_set_t)(context_t *);

typedef struct {
	char short_name;
	const char *long_name;
	const char *description;
	option_set_t set;
	arguments_t arguments;
	arguments_t option_arguments;
} option_t;

void option_init(option_t *, char, const char *, const char *, option_set_t);

void option_add_argument(option_t *, const char *, const char *);
void option_add_option_argument(option_t *, const char *, const char *);

void option_destroy(option_t *);

#endif /* end of include guard: OPTION_H_Q9N3AOUN */
