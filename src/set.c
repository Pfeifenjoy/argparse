#include "argparse/set.h"
#include "argparse/memory.h"

#include "errno.h"
#include "search.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void generic_set_init(generic_set_t *set, uint16_t element_size, compare_t compare) {
	set->root = NULL;
	set->compare = compare;
	generic_vector_init(&set->data, element_size);
}

void generic_set_add(generic_set_t *set, void *element) {
	if(generic_set_has(set, element)) {
		return;
	}
	void *element_copy = generic_vector_add(&set->data, element);
	void **result = tsearch(element_copy, &set->root, set->compare);

	if(result == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

const void *generic_set_find(
		const generic_set_t *set, const void *element, compare_t compare) {
	void **result = (void **) tfind(element, &set->root, compare);
	if(result == NULL) {
		return NULL;
	}
	return *result;
}

const void *generic_set_get_const(const generic_set_t *set, const void *element) {
	return generic_set_find(set, element, set->compare);
}

bool generic_set_has(const generic_set_t *set, const void *element) {
	return generic_set_get_const(set, element) != NULL;
}

void generic_set_destroy(generic_set_t *set) {
	for(size_t i = 0; i < set->data.length; ++i) {
		tdelete(generic_vector_get(&set->data, i), &set->root, set->compare);
	}
	generic_vector_destroy(&set->data);
}
