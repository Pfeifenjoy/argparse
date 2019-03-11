#include "argparse/argument.h"

#include "stdlib.h"

bool argument_test(const struct argument *argument, const char *value) {
	return argument->test == NULL || argument->test(value);
}
