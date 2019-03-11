#include "argparse/command.h"
#include "argparse/memory.h"

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "assert.h"

/************************************************************/
/* Local structs                                            */
/************************************************************/

struct parse_context {
	size_t position;
	size_t argc;
	const char **argv;
};

/************************************************************/
/* Help                                                     */
/************************************************************/
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

void command_print_help(const struct command *command) {
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

/************************************************************/
/* Default handlers                                         */
/************************************************************/

static void help_set(const struct context *context) {
	command_print_help(context->command);
	exit(EXIT_SUCCESS);
}

static void version_set(const struct context *context) {
	printf("%s\n", context->command->version);
	exit(EXIT_SUCCESS);
}

/************************************************************/
/* setters                                                  */
/************************************************************/

void command_flag(
		struct command * command,
		char short_name,
		const char *long_name,
		const char *description,
		optional_set_t set
	) {
	command_optional(
		command,
		short_name,
		long_name,
		description,
		set
	);
}

void command_optional(
		struct command * command,
		char short_name,
		const char *long_name,
		const char *description,
		optional_set_t set
	) {
	struct optional optional;
	optional_init(&optional);
	optional.short_name = short_name;
	optional.long_name = long_name;
	optional.description = description;
	optional.set = set;
	optionals_add(&command->optionals, optional);
}

void command_argument(
		struct command *command,
		char *name
	) {
	struct argument argument;
	argument.name = name;
	command_add_argument(command, argument);
}

void command_add_argument(
		struct command *command,
		struct argument argument
	) {
	arguments_add(&command->arguments, argument);
}

void command_optional_argument(
		struct command *command,
		char *name
	) {
	struct argument argument;
	argument.name = name;
	command_add_optional_argument(command, argument);
}

void command_add_optional_argument(
		struct command *command,
		struct argument argument
	) {
	arguments_add(&command->optional_arguments, argument);
}

/************************************************************/
/* Helper functions                                         */
/************************************************************/

static void optional_check_argument(
		const struct optional *optional,
		const struct argument *argument,
		const char *value
	) {

	if(value[0] == '-') {
		fprintf(stderr, "Unexpected option: %s", value);
		exit(EXIT_FAILURE);
	} else if(argument->test != NULL && !argument->test(value)) {
		fprintf(stderr, "Unexpected argument <%s> for --%s.",
				argument->name, optional->long_name);
		exit(EXIT_FAILURE);
	}
}

static void command_optional_parse_arguments(
		const struct command *command,
		const struct optional *optional,
		struct parse_context *context
	) {
	struct s_vector arguments;
	s_vector_init(&arguments);

	//required arguments
	for(size_t i = 0; i < optional->arguments.size; ++i) {
		const char *current = context->argv[context->position + i];

		if(context->position + i >= context->argc) {
			fprintf(stderr, "Missing argument <%s> for --%s.",
					optional->arguments.data[i].name, optional->long_name);
			exit(EXIT_FAILURE);
		}

		optional_check_argument(optional, optional->arguments.data + i, current);
		s_vector_add(&arguments, current);
	}
	context->position += optional->arguments.size;

	//optional arguments
	for(size_t i = 0; i < optional->optional_arguments.size; ++i) {

		const char *current = context->argv[context->position + i];

		if(context->position + i >= context->argc || current[0] == '-') {
			break;
		}

		optional_check_argument(optional, optional->arguments.data + i, current);
		s_vector_add(&arguments, current);

		context->position++;
	}


	struct context o_set_context = {
		.command = command,
		.arguments = &arguments
	};

	optional->set(&o_set_context);

	s_vector_destroy(&arguments);
}

static void command_parse_optional(
		const struct command *command,
		struct parse_context *context
	) {
	bool long_optional = context->argv[context->position][1] == '-';
	const char *name = context->argv[context->position]
		+ (long_optional ? 2 : 1);

	for(size_t i = 0; i < command->optionals.size; ++i) {
		struct optional *optional = &command->optionals.data[i];
		if((long_optional && strcmp(name, optional->long_name))
		|| (!long_optional && *name == optional->short_name)) {
			//found match

			context->position++;
			command_optional_parse_arguments(command, optional, context);

			return;
		}
	}

	printf("Unknown option: %s\n", context->argv[context->position]);
	exit(EXIT_FAILURE);
}

/************************************************************/
/* General functions                                        */
/************************************************************/

void command_parse(
		const struct command *command,
		size_t argc,
		const char **argv
	) {

	struct parse_context context = {
		.position = 0,
		.argc = argc,
		.argv = argv
	};

	size_t argument_position = 0;
	struct s_vector arguments;
	s_vector_init(&arguments);

	for(context.position = 1; context.position < context.argc;) {
		const char *current = context.argv[context.position];

		if(current[0] == '-') {
			command_parse_optional(command, &context);
		} else if(argument_position >= command->arguments.size
				|| !argument_test(command->arguments.data + argument_position, current)) {
			fprintf(stderr, "Unexpected argument %s", current);
			exit(EXIT_FAILURE);
		} else {
			argument_position++;
			s_vector_add(&arguments, current);
		}
	}

	struct context command_context = {
		.command = command,
		.arguments = &arguments
	};
	command->set(&command_context);

	s_vector_destroy(&arguments);
}

/************************************************************/
/* Allocation functions                                     */
/************************************************************/

void command_init(
		struct command * command,
		const char *name,
		const char *version
	) {
	command->name = name;
	command->version = version;
	optionals_init(&command->optionals);
	arguments_init(&command->arguments);
	arguments_init(&command->optional_arguments);
	command_flag(command, 'h', "help", "Display help message.", help_set);
	command_flag(command, 'V', "version", "Display the version.", version_set);
}


void command_destroy(struct command *command) {
	optionals_destroy(&command->optionals);
	arguments_destroy(&command->arguments);
	arguments_destroy(&command->optional_arguments);
}
