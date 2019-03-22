#include "argparse/vector.h"
#include "argparse/memory.h"

#include "string.h"
#include "assert.h"

void generic_vector_init(generic_vector_t *vector, size_t element_size) {
	assert(element_size != 0);
	vector->data = NULL;
	vector->length = 0;
	vector->element_size = element_size;
}

void *generic_vector_add(generic_vector_t *vector, const void *element) {
	void *element_copy = copy(element, vector->element_size);
	vector->length++;
	vector->data = (void **) reallocate(vector->data, vector->length, sizeof(void *));
	vector->data[vector->length - 1] = element_copy;
	return vector->data[vector->length - 1];
}

void *generic_vector_get(generic_vector_t *vector, size_t i) {
	assert(i < vector->length);
	return vector->data[i];
}

const void *generic_vector_get_const(const generic_vector_t *vector, size_t i) {
	assert(i < vector->length);
	return vector->data[i];
}

void generic_vector_destroy(generic_vector_t *vector) {
	for(size_t i = 0; i < vector->length; ++i) {
		free(generic_vector_get(vector, i));
	}
	free(vector->data);
	vector->data = NULL;
	vector->length = 0;
}

void s_vector_init(s_vector_t *vector) {
	generic_vector_init(vector, sizeof(char *));
}

void s_vector_add(s_vector_t *vector, const char *element) {
	char *element_copy = (char *) allocate(strlen(element) + 1, sizeof(char));
	strcpy(element_copy, element);
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

void c_vector_init(c_vector_t *vector) {
	generic_vector_init(vector, sizeof(char));
}

void c_vector_add(c_vector_t *vector, char element) {
	generic_vector_add(vector, &element);
}

char c_vector_get(const c_vector_t *vector, size_t i) {
	return *((char *) generic_vector_get_const(vector, i));
}

void c_vector_destroy(c_vector_t *vector) {
	generic_vector_destroy(vector);
}
