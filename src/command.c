#include "argparse/command.h"
#include "argparse/memory.h"

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "assert.h"

static void print_description(const char *description) {
	int i = 0;
	int start = 0;

	while(description[i] != '\0') {
		if(description[i] == '\n') {
			printf("%.*s", i - start, description + start);
			printf("\n%*s", MAX_OPTIONAL_NAME + 8, " ");
			start = i + 1;
		}
		i++;
	}

	printf("%.*s", i - start, description + start);

	printf("\n");
}

static void print_help(const struct command *command) {
	printf(
		"\n"
		"  Usage: %s [options]"
		, command->name
	);

	for(size_t i = 0; i < command->arguments.size; ++i) {
		printf(" <%s>", command->arguments.data[i].name);
	}

	printf(
		"\n"
		"\n"
		"  Options:\n"
		"\n"
	);

	for(size_t i = 0; i < command->optionals.size; ++i) {
		const char short_name = command->optionals.data[i].short_name;
		const char *long_name = command->optionals.data[i].long_name;
		const char *description = command->optionals.data[i].description;

		assert(strlen(long_name) < MAX_OPTIONAL_NAME);

		printf(
			"  -%c, --%s%*s",
			short_name,
			long_name,
			(int) (MAX_OPTIONAL_NAME - strlen(long_name)),
			" "
		);

		print_description(description);
	}
	printf("\n");
}

static void help_set(
		const struct command *command,
		const struct optional_arguments *arguments
	) {
	print_help(command);
	exit(EXIT_SUCCESS);
}

static void version_set(
		const struct command *command,
		const struct optional_arguments *arguments
	) {
	printf("%s\n", command->version);
	exit(EXIT_SUCCESS);
}

void command_init(
		struct command * command,
		const char *name,
		const char *version
	) {
	command->name = name;
	command->version = version;
	optionals_init(&command->optionals);
	arguments_init(&command->arguments);
	command_flag(command, 'h', "help", "Display help message.", help_set);
	command_flag(command, 'V', "version", "Display the version.", version_set);
}

void command_flag(
		struct command * command,
		char short_name,
		const char *long_name,
		const char *description,
		optional_set_t optional_set
	) {
	command_optional(
		command,
		short_name,
		long_name,
		description,
		0,
		optional_set
	);
}

void command_optional(
		struct command * command,
		char short_name,
		const char *long_name,
		const char *description,
		size_t arguments,
		optional_set_t optional_set
	) {
	struct optional optional;
	optional.short_name = short_name;
	optional.long_name = long_name;
	optional.description = description;
	optional.optional_set = optional_set;
	optional.arguments = arguments;
	optionals_add(&command->optionals, optional);
}

void command_argument(
		struct command *command,
		const char *name,
		enum argument_type_t type
	) {
	struct argument argument;
	argument.name = name;
	argument.type = type;
	arguments_add(&command->arguments, argument);
}

void command_parse_optional(
		const struct command *command,
		size_t *position,
		size_t argc,
		char * const argv[]
	) {
	bool long_optional = argv[*position][1] == '-';
	const char *name = argv[*position]
		+ (long_optional ? 2 : 1);

	for(size_t i = 0; i < command->optionals.size; ++i) {
		if((long_optional && strcmp(name, command->optionals.data[i].long_name))
		|| (!long_optional && *name == command->optionals.data[i].short_name)) {
			//found match

			struct optional_arguments arguments;
			arguments.argc = command->optionals.data[i].arguments;
			arguments.argv = argv + *position + 1;

			//check that all arguments are "real arguments"
			//and not optional arguments
			if(arguments.argc + *position >= argc) {
				fprintf(stderr, "Optional argument --%s requires %zu arguments.\n",
					command->optionals.data[i].long_name, arguments.argc);
				exit(EXIT_FAILURE);
			}
			for(size_t j = 1; j <= arguments.argc; ++j) {
				if(argv[*position + j][0] == '-') {
					fprintf(stderr,
						"Unexpected parameter %s for optional argument --%s.\n",
						argv[*position + j], command->optionals.data[i].long_name);
					exit(EXIT_FAILURE);
				}
			}

			//call callback
			command->optionals.data[i].optional_set(command, &arguments);

			//increment position
			*position += arguments.argc;

			return;
		}
	}

	printf("Unknown optional argument %s.\n", argv[*position]);
	exit(EXIT_FAILURE);
}

void command_parse(
		const struct command *command,
		size_t argc,
		char * const argv[]
	) {
	for(size_t i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			command_parse_optional(command, &i, argc, argv);
		} else {

		}
	}
}

void command_destroy(struct command *command) {
	optionals_destroy(&command->optionals);
	arguments_destroy(&command->arguments);
}
