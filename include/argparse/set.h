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

void generic_set_destroy(generic_set_t *);

typedef generic_set_t s_set_t;

void s_set_init(s_set_t *);

void s_set_add(s_set_t *, const char *);

bool s_set_has(const s_set_t *, const char *);

void s_set_destroy(s_set_t *);

typedef generic_set_t c_set_t;

void c_set_init(c_set_t *);

void c_set_add(c_set_t *, char);

bool c_set_has(const c_set_t *, char);

void c_set_destroy(c_set_t *);

#endif /* end of include guard: SET_H_GORERFX6 */
