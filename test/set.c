#include "argparse/set.h"

#include "assert.h"
#include "stdlib.h"

static int compare_int(const void *lhs, const void *rhs) {
	int left = *((int *) lhs);
	int right = *((int *) rhs);
	return left - right;
}

static void test_add() {
	generic_set_t set;
	generic_set_init(&set, sizeof(int), compare_int);
	int data = 42;
	assert(!generic_set_has(&set, &data));
	generic_set_add(&set, &data);
	assert(generic_set_has(&set, &data));
	generic_set_destroy(&set);
}

int main() {
	test_add();
	return EXIT_SUCCESS;
}
