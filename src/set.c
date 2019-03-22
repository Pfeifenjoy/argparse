#include "argparse/set.h"
#include "argparse/memory.h"

#include "stdlib.h"
#include "search.h"
#include "string.h"
#include "stdio.h"

void generic_set_init(generic_set_t *set, uint16_t element_size, compare_t compare) {
	set->root = NULL;
	set->compare = compare;
	generic_vector_init(&set->data, element_size);
}

void generic_set_add(generic_set_t *set, void *element) {
	void *element_copy = generic_vector_add(&set->data, element);
	void **result = (void **) tsearch(element_copy, &set->root, set->compare);
	if(result == NULL) {
		fprintf(stderr, "Out of memory.");
		exit(EXIT_FAILURE);
	}
	*result = element_copy;
}

const void *generic_set_get_const(const generic_set_t *set, const void *element) {
	void **result = (void **) tfind(element, &set->root, set->compare);
	if(result == NULL) {
		return NULL;
	} else {
		return *((void **) tfind(element, &set->root, set->compare));
	}
}

bool generic_set_has(const generic_set_t *set, const void *element) {
	return generic_set_get_const(set, element) != NULL;
}

void generic_set_destroy(generic_set_t *set) {
	generic_vector_destroy(&set->data);
	free(set->root);
}

static int s_compare(const void *lhs, const void *rhs) {
	return strcmp(*(const char **) lhs, *(const char **) rhs);
}

void s_set_init(s_set_t *set) {
	generic_set_init(set, sizeof(char *), s_compare);
}

void s_set_add(s_set_t *set, const char *element) {
	char *element_copy = (char *) copy(element, strlen(element) + 1);
	generic_set_add(set, &element_copy);
}

bool s_set_has(const s_set_t *set, const char *element) {
	return generic_set_has(set, &element);
}

void s_set_destroy(s_set_t *set) {
	for(size_t i = 0; i < set->data.length; ++i) {
		free(*((char **) generic_vector_get(&set->data, i)));
	}
	generic_set_destroy(set);
}

static int c_compare(const void *_lhs, const void *_rhs) {
	const char *lhs = (const char *) _lhs;
	const char *rhs = (const char *) _rhs;
	if(*lhs == *rhs) {
		return 0;
	} else if(*lhs < *rhs) {
		return -1;
	} else {
		return 1;
	}
}

void c_set_init(c_set_t *set) {
	generic_set_init(set, sizeof(char), c_compare);
}

void c_set_add(c_set_t *set, char element) {
	generic_set_add(set, &element);
}

bool c_set_has(const c_set_t *set, char element) {
	return generic_set_has(set, &element);
}

void c_set_destroy(c_set_t *set) {
	generic_set_destroy(set);
}
