#ifndef SET_H_GORERFX6
#define SET_H_GORERFX6

#include "argparse/vector.h"

#include "stdbool.h"
#include "stdint.h"

typedef int(*compare_t)(const void *, const void *);

typedef struct {
	generic_vector_t data;
	void *root;
	compare_t compare;
} generic_set_t;

void generic_set_init(generic_set_t *, uint16_t, compare_t);

void generic_set_add(generic_set_t *, void *);

const void *generic_set_get_const(const generic_set_t *, const void *);

bool generic_set_has(const generic_set_t *, const void *);

const void *generic_set_find(const generic_set_t *, const void *, compare_t);

void generic_set_destroy(generic_set_t *);

#endif /* end of include guard: SET_H_GORERFX6 */
