#ifndef OPTIONS_H_BKNLKQUC
#define OPTIONS_H_BKNLKQUC

#include "argparse/option.h"

typedef struct {
	option_t *data;
	size_t size;
} options_t;

void options_init(options_t *);
void options_add(options_t *, option_t);
void options_destroy(options_t *);


#endif /* end of include guard: OPTIONS_H_BKNLKQUC */
