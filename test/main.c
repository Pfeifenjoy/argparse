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
		command_parse(&fixture->command, NULL, 2, argv);
		exit(EXIT_FAILURE);
	}
	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	g_test_trap_assert_stdout("0.0.0\n");
}

static void test_help(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "-h" };
		command_parse(&fixture->command, NULL, 2, argv);
	}

	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	g_test_trap_assert_stdout(
		"\n"
		"  Usage: program [options]\n"
		"\n"
		"  Options:\n"
		"\n"
		"  -h, --help                            Display help message.\n"
		"  -V, --version                         Display the version.\n"
		"\n"
	);
}

static void test_unknown_option(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "-a" };
		command_parse(&fixture->command, NULL, 2, argv);
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

static void a_set(context_t *_context) {
	mult_opt_context_t *context = (mult_opt_context_t *) _context->data;
	context->a = true;
}

static void b_set(context_t *_context) {
	mult_opt_context_t *context = (mult_opt_context_t *) _context->data;
	context->b = true;
}

static void test_multiple_option(fixture_t *fixture, gconstpointer _) {
	mult_opt_context_t context;

	command_flag(&fixture->command, 'a', "a", "", a_set);
	command_flag(&fixture->command, 'b', "b", "", b_set);
	const char *argv[] = { "program", "-ab" };
	command_parse(&fixture->command, &context, 2, argv);

	g_assert_true(context.a);
	g_assert_true(context.b);
}

typedef struct {
	bool flag_set;
} long_name_context_t;

static void long_name_set(context_t *_context) {
	long_name_context_t *context = (long_name_context_t *) _context->data;
	context->flag_set = true;
}

static void test_long_option_name(fixture_t *fixture, gconstpointer _) {
	long_name_context_t context;

	command_flag(&fixture->command, 'f', "flag-name", "", long_name_set);
	const char *argv[] = { "program", "--flag-name" };
	command_parse(&fixture->command, &context, 2, argv);

	g_assert_true(context.flag_set);
}

static void handle_option_with_parameter(context_t *context) {
	bool *called = context->data;
	*called = true;

	g_assert_cmpint(context->arguments->length, ==, 1);
	g_assert_cmpstr(s_vector_get_const(context->arguments, 0), ==, "arg1");
}

static void test_option_parameter(fixture_t *fixture, gconstpointer _) {
	option_t option;
	option_init(&option, 'o', "option", "random option", handle_option_with_parameter);
	option_add_required_argument(&option, "arg1", "random argument");
	command_add_option(&fixture->command, option);
	const char *argv[] = { "program", "--option", "arg1" };

	bool called = false;
	command_parse(&fixture->command, &called, 3, argv);

	g_assert_true(called);
}

static void dummy_set(context_t *context) {
	bool *dummy = context->data;
	*dummy = true;
}

static void test_set_function(fixture_t *fixture, gconstpointer _) {
	bool dummy = false;

	fixture->command.set = dummy_set;
	const char *argv[] = { "program" };
	command_parse(&fixture->command, &dummy, 1, argv);

	g_assert_true(dummy);
}

static void test_unknown_long_option(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "--bad-option" };
		command_parse(&fixture->command, NULL, 2, argv);
	}

	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("Unknown option: --bad-option\n");
}

static void command_set_with_argument(context_t *context) {
	g_assert_cmpint(context->arguments->length, ==, 1);
	g_assert_cmpstr(s_vector_get_const(context->arguments, 0), ==, "argument_1");
}

static void test_command_argument(fixture_t *fixture, gconstpointer _) {
	command_require_argument(&fixture->command, "arg1", "Argument number 1");
	fixture->command.set = command_set_with_argument;

	const char *argv[] = { "program", "argument_1" };
	command_parse(&fixture->command, NULL, 2, argv);
}

static void test_unexpected_command_argument(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program", "argument_1" };
		command_parse(&fixture->command, NULL, 2, argv);
		exit(EXIT_SUCCESS);
	}
	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	g_test_trap_assert_stdout("Warning - Unexpected argument: argument_1\n");
}

static void test_command_missing_required_argument(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		const char *argv[] = { "program" };
		command_require_argument(&fixture->command, "arg1", "Argument 1");
		command_parse(&fixture->command, NULL, 1, argv);
		exit(EXIT_SUCCESS);
	}
	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("Missing required argument <arg1>.\n");
}

static void test_help_command_argument(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		command_optional_argument(&fixture->command, "opt1", "Optional Argument 1");
		command_require_argument(&fixture->command, "arg1", "Required Argument 1");
		command_last_arguments(&fixture->command, "rest", "The rest of the arguments");
		const char *argv[] = { "program", "-h" };
		command_parse(&fixture->command, NULL, 2, argv);
	}

	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	g_test_trap_assert_stdout(
		"\n"
		"  Usage: program [options] <arg1> [opt1] [...rest]\n"
		"\n"
		"  Options:\n"
		"\n"
		"  -h, --help                            Display help message.\n"
		"  -V, --version                         Display the version.\n"
		"\n"
	);
}

static void test_command_last_argument_missing(fixture_t *fixture, gconstpointer _) {
	command_last_arguments(&fixture->command, "last", "Last Argument");
	const char *argv[] = { "program" };
	command_parse(&fixture->command, NULL, 1, argv);
}

static void set_last_argument(context_t *context) {
	g_assert_cmpint(context->arguments->length, ==, 2);
	g_assert_cmpstr(s_vector_get_const(context->arguments, 0), ==, "l1");
	g_assert_cmpstr(s_vector_get_const(context->arguments, 1), ==, "l2");
}

static void test_command_last_argument_available(fixture_t *fixture, gconstpointer _) {
	fixture->command.set = set_last_argument;
	command_last_arguments(&fixture->command, "last", "Last Argument");
	const char *argv[] = { "program", "l1", "l2" };
	command_parse(&fixture->command, NULL, 3, argv);
}

static void set_ignore(context_t *context) { }
static void test_option_argument(fixture_t *fixture, gconstpointer _) {
	if(g_test_subprocess()) {
		option_t option;
		option_init(&option, 'o', "option", "Option with argument.", set_ignore);
		option_add_required_argument(&option, "arg1", "Required Option argument");
		option_add_optional_argument(&option, "arg2", "Optional Option argument");
		command_add_option(&fixture->command, option);
		const char *argv[] = { "program", "-h" };
		command_parse(&fixture->command, NULL, 2, argv);
	}

	g_test_trap_subprocess(NULL, 0, 0);
	g_test_trap_assert_passed();
	g_test_trap_assert_stdout(
		"\n"
		"  Usage: program [options]\n"
		"\n"
		"  Options:\n"
		"\n"
		"  -h, --help                            Display help message.\n"
		"  -V, --version                         Display the version.\n"
		"  -o, --option <arg1> [arg2]            Option with argument.\n"
		"\n"
	);
}

static void duplicate_option_set(context_t *context) {
	static bool has_been_set = false;
	g_assert_false(has_been_set);
	has_been_set = true;
}

static void test_option_duplicate(fixture_t *fixture, gconstpointer _) {
	command_flag(&fixture->command, 'a', "arbitrary", "Arbitrary flag.", duplicate_option_set);
	const char *argv[] = { "program", "-aa" };
	command_parse(&fixture->command, NULL, 2, argv);
}

int main(int argc, char **argv) {
	g_test_init(&argc, &argv, NULL);
	g_test_add("/command/version",
			fixture_t, NULL, program_setup, test_version, program_teardown);
	g_test_add("/command/help",
			fixture_t, NULL, program_setup, test_help, program_teardown);
	g_test_add("/option/unknown",
			fixture_t, NULL, program_setup, test_unknown_option, program_teardown);
	g_test_add("/option/multiple",
			fixture_t, NULL, program_setup, test_multiple_option, program_teardown);
	g_test_add("/option/long-name",
			fixture_t, NULL, program_setup, test_long_option_name, program_teardown);
	g_test_add("/option/argument",
			fixture_t, NULL, program_setup, test_option_parameter, program_teardown);
	g_test_add("/command/set",
			fixture_t, NULL, program_setup, test_set_function, program_teardown);
	g_test_add("/option/unknown-long",
			fixture_t, NULL, program_setup, test_unknown_long_option, program_teardown);
	g_test_add("/command/argument",
			fixture_t, NULL, program_setup, test_command_argument, program_teardown);
	g_test_add("/command/unexpected-argument", fixture_t, NULL,
			program_setup, test_unexpected_command_argument, program_teardown);
	g_test_add("/command/missing-required_argument", fixture_t, NULL,
			program_setup, test_command_missing_required_argument, program_teardown);
	g_test_add("/command/last-argument-missing", fixture_t, NULL,
			program_setup, test_command_last_argument_missing, program_teardown);
	g_test_add("/command/last-argument-available", fixture_t, NULL,
			program_setup, test_command_last_argument_available, program_teardown);
	g_test_add("/command/print-help-argument", fixture_t, NULL,
			program_setup, test_help_command_argument, program_teardown);
	g_test_add("/option/help-argument", fixture_t, NULL,
			program_setup, test_option_argument, program_teardown);
	g_test_add("/option/duplicate", fixture_t, NULL,
			program_setup, test_option_duplicate, program_teardown);
	return g_test_run();
}

