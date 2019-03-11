#include "argparse/optional.h"

void optional_init(optional_t *optional) {
	arguments_init(&optional->arguments);
	arguments_init(&optional->optional_arguments);
}

void optional_add_argument(optional_t *optional, argument_t argument) {
	arguments_add(&optional->arguments, argument);
}

void optional_add_optional_argument(optional_t *optional, argument_t argument) {
	arguments_add(&optional->optional_arguments, argument);
}

void optional_destroy(optional_t *optional) {
	arguments_destroy(&optional->arguments);
	arguments_destroy(&optional->optional_arguments);
}
