#include "argparse/subcommands.h"
#include "argparse/command.h"
#include "argparse/memory.h"
#include "string.h"

int compare_key(const char *lhs, const char *rhs, void *data) {
	return strcmp(lhs, rhs);
}

void subcommands_init(subcommands_t *subcommands) {
	subcommands->commands = g_tree_new_full(
		(GCompareDataFunc) compare_key,
		NULL,
		NULL,
		(GDestroyNotify) command_destroy
	);
}

void subcommands_add(subcommands_t *subcommands, command_t *command) {
	g_tree_insert(subcommands->commands, (void *) command->name, command);
}

const command_t *subcommands_find(const subcommands_t *subcommands, const char *key) {
	return g_tree_lookup(subcommands->commands, key);
}

void subcommands_destroy(subcommands_t *subcommands) {
	g_tree_destroy(subcommands->commands);
}
