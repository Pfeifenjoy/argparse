#include "argparse/optionals.h"
#include "argparse/memory.h"

void optionals_init(struct optionals *optionals) {
	optionals->size = 0;
	optionals->data = NULL;
}

void optionals_add(
	struct optionals *optionals,
	struct optional optional
) {
	optionals->size++;
	optionals->data = 
		reallocate(optionals->data, optionals->size, sizeof(struct optional));
	optionals->data[optionals->size - 1] = optional;
}

void optionals_destroy(struct optionals *optionals) {
	optionals->size = 0;
	free(optionals->data);
}
