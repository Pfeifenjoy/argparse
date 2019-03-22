#include "argparse/context.h"

#include "stdio.h"

void context_init(
		context_t *context,
		const command_t *command,
		const s_vector_t *arguments,
		void *data
	) {
	context->command = command;
	context->arguments = arguments;
	context->data = data;
}

void context_destroy(context_t *context) {
}
