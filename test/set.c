#include "argparse/set.h"

#include "stdlib.h"
#include "assert.h"
#include "stdio.h"

static int compare_int(const void *lhs, const void *rhs) {
	int left = *((int *) lhs);
	int right = *((int *) rhs);
	if(left == right) {
		return 0;
	} else if(left < right) {
		return -1;
	} else {
		return 1;
	}
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

static void test_s_set() {
	s_set_t set;
	s_set_init(&set);
	assert(!s_set_has(&set, "Hello World"));
	s_set_add(&set, "Hello World");
	assert(s_set_has(&set, "Hello World"));
	s_set_destroy(&set);
}

static void test_c_set() {
	c_set_t set;
	c_set_init(&set);

	assert(!c_set_has(&set, 'a'));
	c_set_add(&set, 'a');
	assert(c_set_has(&set, 'a'));

	c_set_destroy(&set);
}

int main(int argc, char **argv) {
	test_add();
	test_s_set();
	test_c_set();
	return EXIT_SUCCESS;
}
