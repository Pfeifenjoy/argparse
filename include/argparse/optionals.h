#ifndef OPTIONALS_H_BKNLKQUC
#define OPTIONALS_H_BKNLKQUC

#include "argparse/optional.h"

struct optionals {
	struct optional *data;
	size_t size;
};

void optionals_init(struct optionals *);
void optionals_add(struct optionals *, struct optional);
void optionals_destroy(struct optionals *);


#endif /* end of include guard: OPTIONALS_H_BKNLKQUC */
