#include "argparse/options.h"

#include "assert.h"

void nothing(context_t *context) { }

static void test_add() {
	options_t options;
	options_init(&options);
	option_t option1;
	option_init(&option1, 'a', "random-option", "Random option", nothing);

	assert(!options_find_by_abbreviation(&options, 'a'));
	options_add(&options, &option1);
	assert(options_find_by_abbreviation(&options, 'a'));

	options_destroy(&options);
}

static void test_find() {
	options_t options;
	options_init(&options);
	option_t option1;
	option_init(&option1, 'a', "random-option", "Random option", nothing);

	assert(options_find_by_abbreviation(&options, 'a') == NULL);
	assert(options_find_by_long_name(&options, "random-option") == NULL);
	options_add(&options, &option1);

	const option_t *result = options_find_by_abbreviation(&options, 'a');
	assert(result != NULL);
	assert(result != &option1);
	assert(option_equal(result, &option1));

	result = options_find_by_long_name(&options, "random-option");
	assert(result != NULL);
	assert(result != &option1);
	assert(option_equal(result, &option1));

	options_destroy(&options);
}

static void test_same_reference() {
	options_t options;
	options_init(&options);
	option_t option;
	option_init(&option, 'a', "first", "First option.", nothing);
	options_add(&options, &option);

	assert(options_find_by_abbreviation(&options, 'a')
			== options_find_by_long_name(&options, "first"));

	options_destroy(&options);
}

int main() {
	test_add();
	test_find();
	test_same_reference();
}
