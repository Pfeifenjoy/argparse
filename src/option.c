#include "argparse/option.h"

#include "assert.h"
#include "ctype.h"
#include "string.h"

static bool no_witespaces(const char *text) {
	for(size_t i = 0; text[i] != '\0'; ++i) {
		if(isspace(text[i])) {
			return false;
		}
	}
	return true;
}

void option_init(
		option_t *option,
		char abbreviation,
		const char *long_name,
		const char *description,
		option_set_t set
	) {
	assert(no_witespaces(long_name));
	option->abbreviation = abbreviation;
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
	arguments_add(&option->required_arguments, &argument);
}

void option_add_optional_argument(
		option_t *option,
		const char *name,
		const char *description
	) {
	argument_t argument;
	argument_init(&argument, name, description);
	arguments_add(&option->optional_arguments, &argument);
}

bool option_equal(const option_t *a, const option_t *b) {
	return a->abbreviation == b->abbreviation
		&& strcmp(a->long_name, b->long_name) == 0
		&& strcmp(a->description, b->description) == 0
		&& a->set == b->set
		&& arguments_equal(&a->required_arguments, &b->required_arguments)
		&& arguments_equal(&a->optional_arguments, &b->optional_arguments);
}

void option_destroy(option_t *option) {
	arguments_destroy(&option->required_arguments);
	arguments_destroy(&option->optional_arguments);
}
