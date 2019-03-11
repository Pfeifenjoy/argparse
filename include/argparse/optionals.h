#ifndef OPTIONALS_H_BKNLKQUC
#define OPTIONALS_H_BKNLKQUC

#include "argparse/optional.h"

typedef struct {
	optional_t *data;
	size_t size;
} optionals_t;

void optionals_init(optionals_t *);
void optionals_add(optionals_t *, optional_t);
void optionals_destroy(optionals_t *);


#endif /* end of include guard: OPTIONALS_H_BKNLKQUC */
