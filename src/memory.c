#include "argparse/memory.h"

#include "stdio.h"
#include "string.h"
#include "sys/errno.h"

void *allocate(size_t amount, size_t size) {
	void *memory = calloc(amount, size);
	if(memory == NULL) {
		printf("Could not allocate memory. (%s)", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return memory;
}

void *reallocate(void *data, size_t amount, size_t size) {
	if(data == NULL) {
		return allocate(amount, size);
	}

	void *memory = realloc(data, amount * size);
	if(memory == NULL) {
		printf("Could not reallocate memory. (%s)", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return memory;
}
