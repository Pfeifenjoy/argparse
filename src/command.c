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
} parse_context_t;

/************************************************************/
/* Default handlers                                         */
/************************************************************/

static void help_set(const context_t *context) {
	command_print_help(context->command);
	exit(EXIT_SUCCESS);
}

static void version_set(const context_t *context) {
	printf("%s\n", context->command->version);
	exit(EXIT_SUCCESS);
}

/************************************************************/
/* Helper functions                                         */
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

static void optional_check_argument(
		const optional_t *optional,
		const argument_t *argument,
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
		const command_t *command,
		const optional_t *optional,
		parse_context_t *context
	) {
	s_vector_t arguments;
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


	context_t o_set_context = {
		.command = command,
		.arguments = &arguments
	};

	optional->set(&o_set_context);

	s_vector_destroy(&arguments);
}

static void command_parse_short_optional(
		const command_t *command,
		parse_context_t *context
	) {
	//array of short names
	const char *flags = context->argv[context->position] + 1;
	context->position++;

	for(size_t i = 0; flags[i] != '\0'; ++i) {
		bool matched = false;

		//search for flag
		for(size_t j = 0; j < command->optionals.size; ++j) {
			optional_t *optional = command->optionals.data + j;
			matched = optional->short_name == flags[i];

			if(matched) {
				command_optional_parse_arguments(command, optional, context);
				break;
			}
		}

		//if no match is found -> error
		if(!matched) {
			fprintf(stderr, "Unknown option: -%c\n", flags[i]);
			exit(EXIT_FAILURE);
		}
	}
}

static void command_parse_long_optional(
		const command_t *command,
		parse_context_t *context
	) {
	const char *name = context->argv[context->position] + 2;

	for(size_t i = 0; command->optionals.size; ++i) {
		optional_t *option = command->optionals.data + i;
		if(strcmp(name, option->long_name) == 0) {
			context->position++;
			command_optional_parse_arguments(command, option, context);
			return;
		}
	}

	fprintf(stderr, "Unknown option: --%s\n", name);
	exit(EXIT_FAILURE);
}

static void command_parse_optional(
		const command_t *command,
		parse_context_t *context
	) {
	if(context->argv[context->position][1] == '-') {
		command_parse_long_optional(command, context);
	} else {
		command_parse_short_optional(command, context);
	}
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
	optionals_init(&command->optionals);
	arguments_init(&command->arguments);
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

void command_flag(
		command_t * command,
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
		command_t * command,
		char short_name,
		const char *long_name,
		const char *description,
		optional_set_t set
	) {
	optional_t optional;
	optional_init(&optional);
	optional.short_name = short_name;
	optional.long_name = long_name;
	optional.description = description;
	optional.set = set;
	optionals_add(&command->optionals, optional);
}

void command_argument(
		command_t *command,
		char *name
	) {
	argument_t argument;
	argument.name = name;
	command_add_argument(command, argument);
}

void command_add_argument(
		command_t *command,
		argument_t argument
	) {
	arguments_add(&command->arguments, argument);
}

void command_optional_argument(
		command_t *command,
		char *name
	) {
	argument_t argument;
	argument.name = name;
	command_add_optional_argument(command, argument);
}

void command_add_optional_argument(
		command_t *command,
		argument_t argument
	) {
	arguments_add(&command->optional_arguments, argument);
}

void command_parse(
		const command_t *command,
		size_t argc,
		const char **argv
	) {

	parse_context_t context = {
		.position = 0,
		.argc = argc,
		.argv = argv
	};

	size_t argument_position = 0;
	s_vector_t arguments;
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

	context_t command_context = {
		.command = command,
		.arguments = &arguments
	};

	if(command->set != NULL) {
		command->set(&command_context);
	}

	s_vector_destroy(&arguments);
}

void command_destroy(command_t *command) {
	optionals_destroy(&command->optionals);
	arguments_destroy(&command->arguments);
	arguments_destroy(&command->optional_arguments);
}
