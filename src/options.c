#include "argparse/options.h"
#include "argparse/memory.h"

#include "assert.h"
#include "stdio.h"
#include "string.h"

void options_init(options_t *options) {
	generic_vector_init(&options->data, sizeof(option_t));
	generic_set_init(&options->abbreviation_index,
			sizeof(option_t *), option_abbreviation_compare);
	generic_set_init(&options->long_name_index,
			sizeof(option_t *), option_long_name_compare);
}

void options_add(options_t *options, option_t option) {
	if(options_find_by_abbreviation(options, option.abbreviation)
		|| options_find_by_long_name(options, option.long_name)) {
		return;
	}
	option_t *option_copy = generic_vector_add(&options->data, &option);
	generic_set_add(&options->abbreviation_index, &option_copy);
	generic_set_add(&options->long_name_index, &option_copy);
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

void options_for_each(const options_t *options,
		void(*f)(const option_t *, void *), void *c) {
	generic_vector_for_each(&options->data, (void (*)(const void *, void *)) f, c);
}

void options_destroy(options_t *options) {
	generic_vector_destroy(&options->data);
	generic_set_destroy(&options->abbreviation_index);
	generic_set_destroy(&options->long_name_index);
}
