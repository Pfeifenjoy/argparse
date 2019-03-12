#include "argparse/argument.h"

void argument_init(argument_t *argument, const char *name, const char *description) {
	argument->name = name;
	argument->description = description;
}
