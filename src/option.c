#include "argparse/option.h"

#include "assert.h"
#include "string.h"

void option_init(
		option_t *option,
		char abbreviation,
		const char *long_name,
		const char *description,
		option_set_t set
	) {
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

bool option_equal(const option_t *a, const option_t *b) {
	return a->abbreviation == b->abbreviation
		&& strcmp(a->long_name, b->long_name) == 0
		&& strcmp(a->description, b->description) == 0;
		//TODO
}

int option_abbreviation_compare(const void *_lhs, const void *_rhs) {
	const option_t *lhs = *(const option_t **) _lhs;
	const option_t *rhs = *(const option_t **) _rhs;
	int result = strncmp(&lhs->abbreviation, &rhs->abbreviation, 1);

#ifndef NDEBUG
	if(result == 0) {
		assert(option_equal(lhs, rhs));
	}
#endif

	return result;
}
int option_long_name_compare(const void *_lhs, const void *_rhs) {
	const option_t *lhs = *(const option_t **) _lhs;
	const option_t *rhs = *(const option_t **) _rhs;
	int result = strcmp(lhs->long_name, rhs->long_name);

#ifndef NDEBUG
	if(result == 0) {
		assert(option_equal(lhs, rhs));
	}
#endif

	return result;
}

void option_destroy(option_t *option) {
	arguments_destroy(&option->required_arguments);
	arguments_destroy(&option->optional_arguments);
}
