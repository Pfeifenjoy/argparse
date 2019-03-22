#include "argparse/context.h"

void context_init(
		context_t *context,
		const command_t *command,
		const s_vector_t *arguments,
		void *data
	) {
	context->command = command;
	context->arguments = arguments;
	context->data = data;
	s_set_init(&context->seen_long_options);
	c_set_init(&context->seen_abbreviations);
}

void context_destroy(context_t *context) {
	s_set_destroy(&context->seen_long_options);
	c_set_destroy(&context->seen_abbreviations);
}
