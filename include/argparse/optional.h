#ifndef OPTIONAL_H_Q9N3AOUN
#define OPTIONAL_H_Q9N3AOUN

#include "stdlib.h"

struct optional_arguments {
	char * const *argv;
	size_t argc;
};

struct command;

typedef void(*optional_set_t)(
	const struct command *,
	const struct optional_arguments *
);

struct optional {
	char short_name;
	const char *long_name;
	const char *description;
	size_t arguments;
	optional_set_t optional_set;
};

#endif /* end of include guard: OPTIONAL_H_Q9N3AOUN */
