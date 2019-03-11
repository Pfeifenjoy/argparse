#ifndef CONTEXT_H_RDHFPUN5
#define CONTEXT_H_RDHFPUN5

#include "argparse/vector.h"

typedef struct command command_t;

typedef struct {
	const command_t *command;
	const s_vector_t *arguments;
	void *data;
} context_t;

#endif /* end of include guard: CONTEXT_H_RDHFPUN5 */
