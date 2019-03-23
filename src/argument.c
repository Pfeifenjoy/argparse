#include "argparse/argument.h"

#include "string.h"

void argument_init(argument_t *argument, const char *name, const char *description) {
	argument->name = name;
	argument->description = description;
}

bool argument_equal(const argument_t *lhs, const argument_t *rhs) {
	return strcmp(lhs->name, rhs->name) == 0
		&& strcmp(lhs->description, rhs->description) == 0;
}
