#include "argparse/option.h"

void option_init(
		option_t *option,
		char short_name,
		const char *long_name,
		const char *description,
		option_set_t set
	) {
	option->short_name = short_name;
	option->long_name = long_name;
	option->description = description;
	option->set = set;
	arguments_init(&option->arguments);
	arguments_init(&option->option_arguments);
}

void option_add_argument(option_t *option, const char *name, const char *description) {
	argument_t argument;
	argument.name = name;
	argument.description = description;
	arguments_add(&option->arguments, argument);
}

void option_add_option_argument(option_t *option, argument_t argument) {
	arguments_add(&option->option_arguments, argument);
}

void option_destroy(option_t *option) {
	arguments_destroy(&option->arguments);
	arguments_destroy(&option->option_arguments);
}
