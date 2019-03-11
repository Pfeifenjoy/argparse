#ifndef ARGUMENT_H_TUQ0BVOC
#define ARGUMENT_H_TUQ0BVOC

#include "stdbool.h"

struct argument {
	const char *name;
	bool(*test)(const char *);
};

bool argument_test(const struct argument *, const char *);

#endif /* end of include guard: ARGUMENT_H_TUQ0BVOC */
