#include "argparse/command.h"
#include "argparse/memory.h"

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "assert.h"

/************************************************************/
/* Local structs                                            */
/************************************************************/

typedef struct {
	size_t position;
	size_t argc;
	const char **argv;
	void *data;
	generic_set_t seen;
} parse_context_t;

static int seen_compare(const void *lhs, const void *rhs) {
	return *((const void **) lhs) - *((const void **) rhs);
}

void parse_context_init(parse_context_t *context) {
	generic_set_init(&context->seen, sizeof(void *), seen_compare);
}

void parse_context_destroy(parse_context_t *context) {
	generic_set_destroy(&context->seen);
}

bool parse_context_has_seen(parse_context_t *context, const option_t *option) {
	return generic_set_has(&context->seen, &option);
}

void parse_context_add_seen(parse_context_t *context, const option_t *option) {
	if(parse_context_has_seen(context, option)) {
		fprintf(stderr, "Duplicate option: --%s\n", option->long_name);
		exit(EXIT_FAILURE);
	}
	return generic_set_add(&context->seen, &option);
}

/************************************************************/
/* Default handlers                                         */
/************************************************************/

static void help_set(context_t *context) {
	command_print_help(context->command);
	context_destroy(context);
	exit(EXIT_SUCCESS);
}

static void version_set(context_t *context) {
	printf("%s\n", context->command->version);
	context_destroy(context);
	exit(EXIT_SUCCESS);
}

/************************************************************/
/* Helper functions                                         */
/************************************************************/
static void option_check_argument(const char *value) {
	if(value[0] == '-') {
		fprintf(stderr, "Unexpected option: %s", value);
		exit(EXIT_FAILURE);
	}
}

static void command_option_parse_arguments(
		const command_t *command,
		const option_t *option,
		parse_context_t *context
	) {
	s_vector_t arguments;
	s_vector_init(&arguments);

	//required arguments
	for(size_t i = 0; i < option->required_arguments.length; ++i) {
		const char *current = context->argv[context->position + i];

		if(context->position + i >= context->argc) {
			fprintf(stderr, "Missing argument <%s> for --%s.",
					arguments_get_const(&option->required_arguments, i)->name,
					option->long_name);
			exit(EXIT_FAILURE);
		}

		option_check_argument(current);
		s_vector_add(&arguments, current);
	}
	context->position += option->required_arguments.length;

	//option arguments
	for(size_t i = 0; i < option->optional_arguments.length; ++i) {
		const char *current = context->argv[context->position + i];

		if(context->position + i >= context->argc || current[0] == '-') {
			break;
		}

		option_check_argument(current);
		s_vector_add(&arguments, current);

		context->position++;
	}

	context_t o_set_context;

	context_init(&o_set_context, command, &arguments, context->data);

	option->set(&o_set_context);

	context_destroy(&o_set_context);

	s_vector_destroy(&arguments);
}

static void command_parse_short_option(
		const command_t *command,
		parse_context_t *context
	) {
	//array of short names
	const char *flags = context->argv[context->position] + 1;
	context->position++;

	for(size_t i = 0; flags[i] != '\0'; ++i) {
		//search for flag
		const option_t *result =
			options_find_by_abbreviation(&command->options, flags[i]);

		//if no match is found -> error
		if(!result) {
			fprintf(stderr, "Unknown option: -%c\n", flags[i]);
			exit(EXIT_FAILURE);
		}
		parse_context_add_seen(context, result);
		command_option_parse_arguments(command, result, context);
	}
}

static void command_parse_long_option(
		const command_t *command,
		parse_context_t *context
	) {
	const char *name = context->argv[context->position] + 2;
	context->position++;

	const option_t *result = options_find_by_long_name(&command->options, name);
	if(!result) {
		fprintf(stderr, "Unknown option: --%s\n", name);
		exit(EXIT_FAILURE);
	}

	parse_context_add_seen(context, result);
	command_option_parse_arguments(command, result, context);
}

static void command_parse_option(
		const command_t *command,
		parse_context_t *context
	) {
	if(context->argv[context->position][1] == '-') {
		command_parse_long_option(command, context);
	} else {
		command_parse_short_option(command, context);
	}
}

static void fprint_option_head(FILE *out, const option_t *option) {
	size_t length = strlen(option->long_name);
	for(size_t i = 0; i < option->required_arguments.length; ++i) {
		length += strlen(arguments_get_const(&option->required_arguments, i)->name) + 3;
	}
	for(size_t i = 0; i < option->optional_arguments.length; ++i) {
		length += strlen(arguments_get_const(&option->optional_arguments, i)->name) + 3;
	}
	assert(strlen(option->long_name) < MAX_OPTION_NAME);

	fprintf(out, "  -%c, --%s", option->abbreviation, option->long_name);
	for(size_t i = 0; i < option->required_arguments.length; ++i) {
		fprintf(out, " <%s>",
				arguments_get_const(&option->required_arguments, i)->name);
	}
	for(size_t i = 0; i < option->optional_arguments.length; ++i) {
		fprintf(out, " [%s]",
				arguments_get_const(&option->optional_arguments, i)->name);
	}
	fprintf(out, "%*s", (int) (MAX_OPTION_NAME - length), "");
}

static void fprint_option_description(FILE *out, const option_t *option) {
	const char *description = option->description;

	int i = 0;
	int start = 0;

	while(description[i] != '\0') {
		if(description[i] == '\n') {
			fprintf(out, "%.*s", i - start, description + start);
			fprintf(out, "\n%*s", MAX_OPTION_NAME + 8, " ");
			start = i + 1;
		}
		i++;
	}

	fprintf(out, "%.*s", i - start, description + start);

	fprintf(out, "\n");
}

static void fprint_option(FILE *out, const option_t *option) {
	fprint_option_head(out, option);
	fprint_option_description(out, option);
}

/************************************************************/
/* Interface                                                */
/************************************************************/

void command_init(
		command_t * command,
		const char *name,
		const char *version
	) {
	command->name = name;
	command->version = version;
	command->set = NULL;

	argument_init(&command->last_arguments, "", "");
	options_init(&command->options);
	arguments_init(&command->required_arguments);
	arguments_init(&command->optional_arguments);
	command_flag(command, 'h', "help", "Display help message.", help_set);
	command_flag(command, 'V', "version", "Display the version.", version_set);
}

void command_print_help(const command_t *command) {
	printf(
		"\n"
		"  Usage: %s [options]"
		, command->name
	);

	for(size_t i = 0; i < command->required_arguments.length; ++i) {
		printf(" <%s>",
				arguments_get_const(&command->required_arguments, i)->name);
	}
	for(size_t i = 0; i < command->optional_arguments.length; ++i) {
		printf(" [%s]",
				arguments_get_const(&command->optional_arguments, i)->name);
	}
	if(command->last_arguments.name[0] != '\0') {
		printf(" [...%s]", command->last_arguments.name);
	}

	printf(
		"\n"
		"\n"
		"  Options:\n"
		"\n"
	);

	for(size_t i = 0; i < command->options.data.length; ++i) {
		fprint_option(stdout, options_get(&command->options, i));
	}
	printf("\n");
}

void command_flag(
		command_t * command,
		char abbreviation,
		const char *long_name,
		const char *description,
		option_set_t set
	) {
	command_option(
		command,
		abbreviation,
		long_name,
		description,
		set
	);
}

void command_option(
		command_t * command,
		char abbreviation,
		const char *long_name,
		const char *description,
		option_set_t set
	) {
	option_t option;
	option_init(&option, abbreviation, long_name, description, set);
	command_add_option(command, option);
}

void command_add_option(command_t *command, option_t option) {
	options_add(&command->options, &option);
}

void command_require_argument(
		command_t *command,
		const char *name,
		const char *description
	) {
	argument_t argument;
	argument_init(&argument, name, description);
	command_add_required_argument(command, argument);
}

void command_add_required_argument(
		command_t *command,
		argument_t argument
	) {
	arguments_add(&command->required_arguments, &argument);
}

void command_optional_argument(
		command_t *command,
		const char *name,
		const char *description
	) {
	argument_t argument;
	argument_init(&argument, name, description);
	command_add_optional_argument(command, argument);
}

void command_add_optional_argument(
		command_t *command,
		argument_t argument
	) {
	arguments_add(&command->optional_arguments, &argument);
}

void command_last_arguments(command_t *command, const char *name, const char *description) {
	command->last_arguments.name = name;
	command->last_arguments.description = description;
}

void command_parse(
		const command_t *command,
		void *data,
		size_t argc,
		const char **argv
	) {
	assert(argc >= 1 && "arguments must at least contain the program name.");

	parse_context_t parse_context = {
		.position = 0,
		.argc = argc,
		.argv = argv,
		.data = data
	};

	parse_context_init(&parse_context);

	s_vector_t arguments;
	s_vector_init(&arguments);

	for(parse_context.position = 1; parse_context.position < parse_context.argc;) {
		const char *current = parse_context.argv[parse_context.position];

		if(current[0] == '-') {
			command_parse_option(command, &parse_context);
		} else {
			if(arguments.length >=
				command->optional_arguments.length + command->required_arguments.length
				&& command->last_arguments.name[0] == '\0') {
				printf("Warning - Unexpected argument: %s\n", current);
			}
			s_vector_add(&arguments, current);
			parse_context.position++;
		}
	}

	if(arguments.length < command->required_arguments.length) {
		fprintf(stderr, "Missing required argument <%s>.\n",
			arguments_get_const(&command->required_arguments, arguments.length)->name);
		exit(EXIT_FAILURE);
	}

	context_t command_context;

	context_init(&command_context, command, &arguments, data);

	if(command->set != NULL) {
		command->set(&command_context);
	}

	context_destroy(&command_context);

	s_vector_destroy(&arguments);

	parse_context_destroy(&parse_context);
}

void command_destroy(command_t *command) {
	options_destroy(&command->options);
	arguments_destroy(&command->required_arguments);
	arguments_destroy(&command->optional_arguments);
}
