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
} parse_context_t;

/************************************************************/
/* Default handlers                                         */
/************************************************************/

static void help_set(context_t *context) {
	command_print_help(context->command);
	exit(EXIT_SUCCESS);
}

static void version_set(context_t *context) {
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
			printf("\n%*s", MAX_OPTION_NAME + 8, " ");
			start = i + 1;
		}
		i++;
	}

	printf("%.*s", i - start, description + start);

	printf("\n");
}

static void option_check_argument(
		const option_t *option,
		const char *value
	) {
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
	for(size_t i = 0; i < option->arguments.size; ++i) {
		const char *current = context->argv[context->position + i];

		if(context->position + i >= context->argc) {
			fprintf(stderr, "Missing argument <%s> for --%s.",
					option->arguments.data[i].name, option->long_name);
			exit(EXIT_FAILURE);
		}

		option_check_argument(option, current);
		s_vector_add(&arguments, current);
	}
	context->position += option->arguments.size;

	//option arguments
	for(size_t i = 0; i < option->option_arguments.size; ++i) {
		const char *current = context->argv[context->position + i];

		if(context->position + i >= context->argc || current[0] == '-') {
			break;
		}

		option_check_argument(option, current);
		s_vector_add(&arguments, current);

		context->position++;
	}


	context_t o_set_context = {
		.command = command,
		.arguments = &arguments,
		.data = context->data
	};

	option->set(&o_set_context);

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
		bool matched = false;

		//search for flag
		for(size_t j = 0; j < command->options.size; ++j) {
			option_t *option = command->options.data + j;
			matched = option->short_name == flags[i];

			if(matched) {
				command_option_parse_arguments(command, option, context);
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

static void command_parse_long_option(
		const command_t *command,
		parse_context_t *context
	) {
	const char *name = context->argv[context->position] + 2;
	context->position++;

	for(size_t i = 0; i < command->options.size; ++i) {
		option_t *option = command->options.data + i;
		if(strcmp(name, option->long_name) == 0) {
			command_option_parse_arguments(command, option, context);
			return;
		}
	}

	fprintf(stderr, "Unknown option: --%s\n", name);
	exit(EXIT_FAILURE);
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

	options_init(&command->options);
	arguments_init(&command->arguments);
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

	for(size_t i = 0; i < command->options.size; ++i) {
		const char short_name = command->options.data[i].short_name;
		const char *long_name = command->options.data[i].long_name;
		const char *description = command->options.data[i].description;

		assert(strlen(long_name) < MAX_OPTION_NAME);

		printf(
			"  -%c, --%s%*s",
			short_name,
			long_name,
			(int) (MAX_OPTION_NAME - strlen(long_name)),
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
		option_set_t set
	) {
	command_option(
		command,
		short_name,
		long_name,
		description,
		set
	);
}

void command_option(
		command_t * command,
		char short_name,
		const char *long_name,
		const char *description,
		option_set_t set
	) {
	option_t option;
	option_init(&option, short_name, long_name, description, set);
	command_add_option(command, option);
}

void command_add_option(command_t *command, option_t option) {
	options_add(&command->options, option);
}

void command_argument(
		command_t *command,
		const char *name,
		const char *description
	) {
	argument_t argument;
	argument_init(&argument, name, description);
	command_add_argument(command, argument);
}

void command_add_argument(
		command_t *command,
		argument_t argument
	) {
	arguments_add(&command->arguments, argument);
}

void command_parse(
		const command_t *command,
		void *data,
		size_t argc,
		const char **argv
	) {

	parse_context_t parse_context = {
		.position = 0,
		.argc = argc,
		.argv = argv,
		.data = data
	};

	size_t argument_position = 0;
	s_vector_t arguments;
	s_vector_init(&arguments);

	for(parse_context.position = 1; parse_context.position < parse_context.argc;) {
		const char *current = parse_context.argv[parse_context.position];

		if(current[0] == '-') {
			command_parse_option(command, &parse_context);
		} else if(argument_position >= command->arguments.size) {
			fprintf(stderr, "Unexpected argument %s", current);
			exit(EXIT_FAILURE);
		} else {
			argument_position++;
			s_vector_add(&arguments, current);
		}
	}

	context_t command_context = {
		.command = command,
		.arguments = &arguments,
		.data = data
	};

	if(command->set != NULL) {
		command->set(&command_context);
	}

	s_vector_destroy(&arguments);
}

void command_destroy(command_t *command) {
	options_destroy(&command->options);
	arguments_destroy(&command->arguments);
}
