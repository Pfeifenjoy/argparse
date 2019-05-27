#include "argparse/subcommands.h"
#include "argparse/command.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

static void test_insert() {
	subcommands_t subcommands;

	subcommands_init(&subcommands);

	command_t command;
	command_init(&command, "test", "0.0.0");

	assert(g_tree_nnodes(subcommands.commands) == 0);
	subcommands_add(&subcommands, &command);
	assert(g_tree_nnodes(subcommands.commands) == 1);

	const command_t *found_command = subcommands_find(&subcommands, "test");

	assert(found_command != NULL);
	assert(strcmp(found_command->name, "test") == 0);

	subcommands_destroy(&subcommands);
}

int main() {
	test_insert();
	return EXIT_SUCCESS;
}
