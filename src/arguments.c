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

bool arguments_equal(const arguments_t *lhs, const arguments_t *rhs) {
	if(lhs->size != rhs->size) {
		return false;
	}
	for(size_t i = 0; i < lhs->size; ++i) {
		if(!argument_equal(lhs->data + i, rhs->data +i)) {
			return false;
		}
	}
	return true;
}

void arguments_destroy(arguments_t *arguments) {
	arguments->size = 0;
	free(arguments->data);
}
