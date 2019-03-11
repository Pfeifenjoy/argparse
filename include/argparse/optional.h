#ifndef OPTIONAL_H_Q9N3AOUN
#define OPTIONAL_H_Q9N3AOUN

#include "argparse/vector.h"
#include "argparse/arguments.h"
#include "argparse/context.h"

#include "stdlib.h"

typedef void(*optional_set_t)(const struct context *);

struct optional {
	char short_name;
	const char *long_name;
	const char *description;
	optional_set_t set;
	struct arguments arguments;
	struct arguments optional_arguments;
};

void optional_init(struct optional *);

void optional_add_argument(struct optional *, struct argument);
void optional_add_optional_argument(struct optional *, struct argument);

void optional_destroy(struct optional *);

#endif /* end of include guard: OPTIONAL_H_Q9N3AOUN */
