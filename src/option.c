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
	arguments_init(&option->required_arguments);
	arguments_init(&option->optional_arguments);
}

void option_add_required_argument(
		option_t *option,
		const char *name,
		const char *description
	) {
	argument_t argument;
	argument_init(&argument, name, description);
	arguments_add(&option->required_arguments, argument);
}

void option_add_optional_argument(
		option_t *option,
		const char *name,
		const char *description
	) {
	argument_t argument;
	argument_init(&argument, name, description);
	arguments_add(&option->optional_arguments, argument);
}

void option_destroy(option_t *option) {
	arguments_destroy(&option->required_arguments);
	arguments_destroy(&option->optional_arguments);
}
