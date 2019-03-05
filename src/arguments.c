#include "argparse/arguments.h"
#include "argparse/memory.h"

void arguments_init(struct arguments *arguments) {
	arguments->data = NULL;
	arguments->size = 0;
}

void arguments_add(struct arguments *arguments, struct argument argument) {
	arguments->size++;
	arguments->data =
		reallocate(arguments->data, arguments->size, sizeof(struct argument));
	arguments->data[arguments->size - 1] = argument;
}

void arguments_destroy(struct arguments *arguments) {
	arguments->size = 0;
	free(arguments->data);
}
