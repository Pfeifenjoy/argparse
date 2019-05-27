#ifndef SUBCOMMANDS_H_C4UUQ5FS
#define SUBCOMMANDS_H_C4UUQ5FS

#include "gmodule.h"

typedef struct command command_t;

typedef struct {
	GTree *commands;
} subcommands_t;

void subcommands_init(subcommands_t *);

void subcommands_add(subcommands_t *, command_t *);

const command_t *subcommands_find(const subcommands_t *, const char *);

void subcommands_destroy(subcommands_t *);

#endif /* end of include guard: SUBCOMMANDS_H_C4UUQ5FS */
