#ifndef ARGPARSE_H_QMPV58OX
#define ARGPARSE_H_QMPV58OX

#ifndef MAX_OPTION_NAME
#define MAX_OPTION_NAME (40 - 8)
#endif

#ifndef MIN_DESCRIPTION_LINE
#define MIN_DESCRIPTION_LINE 20
#endif

#include "argparse/options.h"
#include "argparse/arguments.h"
#include "argparse/context.h"

typedef void(*command_set_t)(context_t *);

typedef struct command {
	const char *name;
	const char *version;
	options_t options;
	arguments_t arguments;
	command_set_t set;
} command_t;

void command_init(command_t *, const char *name, const char *version);

void command_flag(
	command_t *,
	char short_name,
	const char *long_name,
	const char *description,
	option_set_t
);

void command_option(
	command_t *,
	char short_name,
	const char *long_name,
	const char *description,
	option_set_t
);

void command_add_option(
	command_t *,
	option_t
);

void command_argument(
	command_t *,
	const char *,
	const char *
);

void command_add_argument(
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
