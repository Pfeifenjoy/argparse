#include "argparse/arguments.h"
#include "argparse/memory.h"

void arguments_init(arguments_t *arguments) {
	generic_vector_init(arguments, sizeof(argument_t));
}

void arguments_add(arguments_t *arguments, argument_t *argument) {
	generic_vector_add(arguments, argument);
}

argument_t *arguments_get(arguments_t *arguments, size_t i) {
	return generic_vector_get(arguments, i);
}

const argument_t *arguments_get_const(const arguments_t *arguments, size_t i) {
	return generic_vector_get_const(arguments, i);
}

bool arguments_equal(const arguments_t *lhs, const arguments_t *rhs) {
	if(lhs->length != rhs->length) {
		return false;
	}
	for(size_t i = 0; i < lhs->length; ++i) {
		if(!argument_equal(arguments_get_const(lhs, i), arguments_get_const(rhs, i))) {
			return false;
		}
	}
	return true;
}

void arguments_destroy(arguments_t *arguments) {
	generic_vector_destroy(arguments);
}
