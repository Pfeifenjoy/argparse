#ifndef ARGPARSE_H_QMPV58OX
#define ARGPARSE_H_QMPV58OX

#ifndef MAX_OPTIONAL_NAME
#define MAX_OPTIONAL_NAME (40 - 8)
#endif

#ifndef MIN_DESCRIPTION_LINE
#define MIN_DESCRIPTION_LINE 20
#endif

#include "argparse/optionals.h"
#include "argparse/arguments.h"
#include "argparse/context.h"

typedef void(*command_set_t)(context_t *);

typedef struct command {
	const char *name;
	const char *version;
	optionals_t optionals;
	arguments_t arguments;
	arguments_t optional_arguments;
	command_set_t set;
} command_t;

void command_init(command_t *, const char *name, const char *version);

void command_flag(
	command_t *,
	char short_name,
	const char *long_name,
	const char *description,
	optional_set_t
);

void command_optional(
	command_t *,
	char short_name,
	const char *long_name,
	const char *description,
	optional_set_t
);

void command_argument(
	command_t *,
	char *
);

void command_add_argument(
	command_t *,
	argument_t
);

void command_optional_argument(
	command_t *,
	char *
);

void command_add_optional_argument(
	command_t *,
	argument_t
);

void command_parse(
	const command_t *,
	void *,
	size_t argc,
	const char **argv
);

void command_print_help(const command_t *);

void command_destroy(command_t *);

#endif /* end of include guard: ARGPARSE_H_QMPV58OX */
