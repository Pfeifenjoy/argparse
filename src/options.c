#include "argparse/options.h"
#include "argparse/memory.h"

#include "assert.h"
#include "stdio.h"
#include "string.h"


static int abbreviation_compare(const void *_lhs, const void *_rhs) {
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

static int long_name_compare(const void *_lhs, const void *_rhs) {
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

void options_init(options_t *options) {
	generic_vector_init(&options->data, sizeof(option_t));
	generic_set_init(&options->abbreviation_index,
			sizeof(option_t *), abbreviation_compare);
	generic_set_init(&options->long_name_index,
			sizeof(option_t *), long_name_compare);
}

option_t *options_add(options_t *options, option_t *option) {

#ifndef NDEBUG
	if(options_find_by_abbreviation(options, option->abbreviation)
		|| options_find_by_long_name(options, option->long_name)) {
		assert(false && "Cannot add option twice");
	}
#endif

	option_t *option_copy = generic_vector_add(&options->data, option);
	generic_set_add(&options->abbreviation_index, &option_copy);
	generic_set_add(&options->long_name_index, &option_copy);
	return option_copy;
}

const option_t *options_get(const options_t *options, size_t i) {
	return generic_vector_get_const(&options->data, i);
}

static int find_abbreviation(const void *_lhs, const void *_rhs) {
	const char *lhs = (const char *) _lhs;
	const option_t *rhs = *(const option_t **) _rhs;

	return strncmp(lhs, &rhs->abbreviation, 1);
}

const option_t *options_find_by_abbreviation(
		const options_t *options, char abbreviation) {
	const option_t **result = (const option_t **)
		generic_set_find(&options->abbreviation_index,
			&abbreviation, find_abbreviation);
	if(!result) {
		return NULL;
	}
	return *result;
}

static int find_long_name(const void *_lhs, const void *_rhs) {
	const char *lhs = (const char *) _lhs;
	const option_t *rhs = *(const option_t **) _rhs;
	return strcmp(lhs, rhs->long_name);
}

const option_t *options_find_by_long_name(
		const options_t *options, const char *abbreviation) {
	const option_t **result = (const option_t **)
		generic_set_find(&options->long_name_index, abbreviation, find_long_name);
	if(!result) {
		return NULL;
	}
	return *result;
}

void options_destroy(options_t *options) {
	for(size_t i = 0; i < options->data.length; ++i) {
		option_destroy(generic_vector_get(&options->data, i));
	}
	generic_vector_destroy(&options->data);
	generic_set_destroy(&options->abbreviation_index);
	generic_set_destroy(&options->long_name_index);
}
