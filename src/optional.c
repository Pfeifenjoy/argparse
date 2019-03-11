#include "argparse/optional.h"

void optional_init(struct optional *optional) {
	arguments_init(&optional->arguments);
	arguments_init(&optional->optional_arguments);
}

void optional_add_argument(struct optional *optional, struct argument argument) {
	arguments_add(&optional->arguments, argument);
}

void optional_add_optional_argument(struct optional *optional, struct argument argument) {
	arguments_add(&optional->optional_arguments, argument);
}

void optional_destroy(struct optional *optional) {
	arguments_destroy(&optional->arguments);
	arguments_destroy(&optional->optional_arguments);
}
