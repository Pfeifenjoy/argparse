#ifndef OPTIONS_H_BKNLKQUC
#define OPTIONS_H_BKNLKQUC

#include "argparse/option.h"
#include "argparse/set.h"

typedef struct options {
	generic_vector_t data;
	generic_set_t abbreviation_index;
	generic_set_t long_name_index;
} options_t;

void options_init(options_t *);
void options_add(options_t *, option_t);
const option_t *options_find_by_abbreviation(const options_t *, char);
const option_t *options_find_by_long_name(const options_t *, const char *);
void options_for_each(const options_t *, void(*)(const option_t *, void *), void *);
void options_destroy(options_t *);

#endif /* end of include guard: OPTIONS_H_BKNLKQUC */
