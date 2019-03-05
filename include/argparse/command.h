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

struct command {
	const char *name;
	const char *version;
	void *context;
	struct optionals optionals;
	struct arguments arguments;
};

void command_init(struct command *, const char *name, const char *version);

void command_flag(
	struct command *,
	char short_name,
	const char *long_name,
	const char *description,
	optional_set_t
);

void command_optional(
	struct command *,
	char short_name,
	const char *long_name,
	const char *description,
	size_t arguments,
	optional_set_t
);

void command_argument(
	struct command *,
	const char *name,
	enum argument_type_t type
);

void command_parse(
	const struct command *,
	size_t argc,
	char * const argv[]
);

void command_destroy(struct command *);

#endif /* end of include guard: ARGPARSE_H_QMPV58OX */
