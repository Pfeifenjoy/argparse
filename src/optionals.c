#include "argparse/optionals.h"
#include "argparse/memory.h"

void optionals_init(optionals_t *optionals) {
	optionals->size = 0;
	optionals->data = NULL;
}

void optionals_add(
	optionals_t *optionals,
	optional_t optional
) {
	optionals->size++;
	optionals->data = (optional_t *)
		reallocate(optionals->data, optionals->size, sizeof(optional_t));
	optionals->data[optionals->size - 1] = optional;
}

void optionals_destroy(optionals_t *optionals) {
	for(size_t i = 0; i < optionals->size; ++i) {
		optional_destroy(optionals->data + i);
	}
	optionals->size = 0;
	free(optionals->data);
}
