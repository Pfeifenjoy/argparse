#ifndef ARGUMENT_H_TUQ0BVOC
#define ARGUMENT_H_TUQ0BVOC

#include "stdbool.h"

typedef struct {
	const char *name;
	bool(*test)(const char *);
} argument_t;

bool argument_test(const argument_t *, const char *);

#endif /* end of include guard: ARGUMENT_H_TUQ0BVOC */
