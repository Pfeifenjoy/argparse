#ifndef CONTEXT_H_RDHFPUN5
#define CONTEXT_H_RDHFPUN5

#include "argparse/vector.h"
#include "argparse/set.h"

typedef struct command command_t;

typedef struct {
	const command_t *command;
	const s_vector_t *arguments;
	void *data;
	s_set_t seen_long_options;
	c_set_t seen_abbreviations;
} context_t;

void context_init(context_t *, const command_t *, const s_vector_t *, void *);

void context_destroy(context_t *);

#endif /* end of include guard: CONTEXT_H_RDHFPUN5 */
