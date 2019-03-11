#ifndef CONTEXT_H_RDHFPUN5
#define CONTEXT_H_RDHFPUN5

#include "argparse/vector.h"

struct command;
struct context {
	const struct command *command;
	const struct s_vector *arguments;
};

#endif /* end of include guard: CONTEXT_H_RDHFPUN5 */
