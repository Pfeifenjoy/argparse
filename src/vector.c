#include "argparse/vector.h"
#include "argparse/memory.h"

#include "string.h"

void s_vector_init(struct s_vector *vector) {
	vector->data = NULL;
	vector->length = 0;
}

void s_vector_add(struct s_vector *vector, const char *element) {
	char *my_element = (char *) allocate(strlen(element) + 1, sizeof(char));
	strcpy(my_element, element);
	vector->length++;
	vector->data = (char **) reallocate(vector->data, vector->length, sizeof(char *));
	vector->data[vector->length - 1] = my_element;
}

void s_vector_destroy(struct s_vector *vector) {
	for(size_t i = 0; i < vector->length; ++i) {
		free(vector->data[i]);
	}
	free(vector->data);
	vector->length = 0;
}
