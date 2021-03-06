#ifndef ARGUMENT_H_TUQ0BVOC
#define ARGUMENT_H_TUQ0BVOC

#include "stdbool.h"


typedef struct {
	const char *name;
	const char *description;
} argument_t;

void argument_init(argument_t *argument, const char *, const char *);
bool argument_equal(const argument_t *, const argument_t *);

#endif /* end of include guard: ARGUMENT_H_TUQ0BVOC */
