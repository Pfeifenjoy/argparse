#ifndef OPTION_H_Q9N3AOUN
#define OPTION_H_Q9N3AOUN

#include "argparse/vector.h"
#include "argparse/arguments.h"
#include "argparse/context.h"

#include "stdlib.h"
#include "stdbool.h"

typedef void(*option_set_t)(context_t *);

typedef struct {
	char short_name;
	const char *long_name;
	const char *description;
	option_set_t set;
	bool already_set;
	arguments_t required_arguments;
	arguments_t optional_arguments;
} option_t;

void option_init(option_t *, char, const char *, const char *, option_set_t);

void option_add_required_argument(option_t *, const char *, const char *);
void option_add_optional_argument(option_t *, const char *, const char *);

void option_destroy(option_t *);

#endif /* end of include guard: OPTION_H_Q9N3AOUN */
