#include "argparse/options.h"
#include "argparse/memory.h"

void options_init(options_t *options) {
	options->size = 0;
	options->data = NULL;
}

void options_add(
	options_t *options,
	option_t option
) {
	options->size++;
	options->data = (option_t *)
		reallocate(options->data, options->size, sizeof(option_t));
	options->data[options->size - 1] = option;
}

void options_destroy(options_t *options) {
	for(size_t i = 0; i < options->size; ++i) {
		option_destroy(options->data + i);
	}
	options->size = 0;
	free(options->data);
}
