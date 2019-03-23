#include "argparse/vector.h"
#include "argparse/memory.h"

#ifndef VECTOR_BLOCK_SIZE
#define VECTOR_BLOCK_SIZE 16
#endif

#include "assert.h"
#include "string.h"

void generic_vector_init(generic_vector_t *vector, size_t element_size) {
	assert(element_size > 0);
	vector->blocks = NULL;
	vector->length = 0;
	vector->element_size = element_size;
}

static void add_block(generic_vector_t *vector) {
	size_t length = (vector->length + 1) / VECTOR_BLOCK_SIZE
		+ ((vector->length + 1) % VECTOR_BLOCK_SIZE ? 1 : 0);
	vector->blocks = reallocate(vector->blocks, length, sizeof(void *));
	vector->blocks[length - 1] = allocate(VECTOR_BLOCK_SIZE, vector->element_size);
}

void *generic_vector_add(generic_vector_t *vector, const void *element) {
	if(vector->length % VECTOR_BLOCK_SIZE == 0) {
		add_block(vector);
	}
	vector->length++;
	void *entry = generic_vector_get(vector, vector->length - 1);
	memcpy(entry, element, vector->element_size);
	return entry;
}

void *generic_vector_get(generic_vector_t *vector, size_t i) {
	assert(i < vector->length);
	return vector->blocks[i / VECTOR_BLOCK_SIZE] + (i % VECTOR_BLOCK_SIZE)
		* vector->element_size;
}

const void *generic_vector_get_const(const generic_vector_t *vector, size_t i) {
	return generic_vector_get((generic_vector_t *) vector, i);
}

void generic_vector_destroy(generic_vector_t *vector) {
	size_t length = vector->length / VECTOR_BLOCK_SIZE
		+ (vector->length % VECTOR_BLOCK_SIZE ? 1 : 0);
	for(size_t i = 0; i < length; ++i) {
		free(vector->blocks[i]);
	}
	free(vector->blocks);
}

void s_vector_init(s_vector_t *vector) {
	generic_vector_init(vector, sizeof(char *));
}

void s_vector_add(s_vector_t *vector, const char *element) {
	const size_t length = strlen(element) + 1;
	char *element_copy = (char *) copy(element, length);
	generic_vector_add(vector, &element_copy);
}

char *s_vector_get(s_vector_t *vector, size_t i) {
	return *((char **) generic_vector_get(vector, i));
}

const char *s_vector_get_const(const s_vector_t *vector, size_t i) {
	return *((char **) generic_vector_get_const(vector, i));
}

void s_vector_destroy(s_vector_t *vector) {
	for(size_t i = 0; i < vector->length; ++i) {
		free(s_vector_get(vector, i));
	}
	generic_vector_destroy(vector);
}
