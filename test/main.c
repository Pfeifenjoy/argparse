#include "argparse/command.h"
#include "argparse/memory.h"

#include "glib.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "assert.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"

typedef struct {
	command_t command;
} fixture_t;

static void program_setup(fixture_t *fixture, gconstpointer _) {
	command_init(&fixture->command, "program", "0.0.0");
}

static void program_teardown(fixture_t *fixture, gconstpointer _) {
	command_destroy(&fixture->command);
}

static void test_version(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "-V" };
		command_parse(&fixture->command, 2, argv);
	}
	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	g_test_trap_assert_stdout("0.0.0\n");
}

static void test_help(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "-h" };
		command_parse(&fixture->command, 2, argv);
	}

	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	FILE *expected = fopen("basic-help", "r");
	char buffer[165];
	g_assert_true(fread(buffer, sizeof(char), 165, expected));
	g_test_trap_assert_stdout(buffer);
}

static void test_unknown_option(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "-a" };
		command_parse(&fixture->command, 2, argv);
	}

	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("Unknown option: -a\n");
	g_test_trap_assert_stdout("");
}

typedef struct {
	bool a;
	bool b;
} mult_opt_context_t;

static void a_set(const context_t *_context) {
	mult_opt_context_t *context = (mult_opt_context_t *) _context->command->context;
	context->a = true;
}

static void b_set(const context_t *_context) {
	mult_opt_context_t *context = (mult_opt_context_t *) _context->command->context;
	context->b = true;
}

static void test_multiple_option(fixture_t *fixture, gconstpointer _) {
	mult_opt_context_t context;
	fixture->command.context = &context;

	command_flag(&fixture->command, 'a', "a", "", a_set);
	command_flag(&fixture->command, 'b', "b", "", b_set);
	const char *argv[] = { "program", "-ab" };
	command_parse(&fixture->command, 2, argv);

	g_assert_true(context.a);
	g_assert_true(context.b);
}

typedef struct {
	bool flag_set;
} long_name_context_t;

static void long_name_set(const context_t *_context) {
	long_name_context_t *context = (long_name_context_t *) _context->command->context;
	context->flag_set = true;
}

static void test_long_option_name(fixture_t *fixture, gconstpointer _) {
	long_name_context_t context;
	fixture->command.context = &context;

	command_flag(&fixture->command, 'f', "flag-name", "", long_name_set);
	const char *argv[] = { "program", "--flag-name" };
	command_parse(&fixture->command, 2, argv);

	g_assert_true(context.flag_set);
}

int main(int argc, char **argv) {
	g_test_init(&argc, &argv, NULL);
	g_test_add("/command/version",
			fixture_t, NULL, program_setup, test_version, program_teardown);
	g_test_add("/command/help",
			fixture_t, NULL, program_setup, test_help, program_teardown);
	g_test_add("/command/unknown-option",
			fixture_t, NULL, program_setup, test_unknown_option, program_teardown);
	g_test_add("/command/multiple-flags",
			fixture_t, NULL, program_setup, test_multiple_option, program_teardown);
	g_test_add("/command/long-option-name",
			fixture_t, NULL, program_setup, test_long_option_name, program_teardown);
	return g_test_run();
}

