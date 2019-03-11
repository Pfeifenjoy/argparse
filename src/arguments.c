#include "argparse/arguments.h"
#include "argparse/memory.h"

void arguments_init(arguments_t *arguments) {
	arguments->data = NULL;
	arguments->size = 0;
}

void arguments_add(arguments_t *arguments, argument_t argument) {
	arguments->size++;
	arguments->data =
		reallocate(arguments->data, arguments->size, sizeof(argument_t));
	arguments->data[arguments->size - 1] = argument;
}

void arguments_destroy(arguments_t *arguments) {
	arguments->size = 0;
	free(arguments->data);
}
