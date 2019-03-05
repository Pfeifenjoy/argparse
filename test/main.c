#include "argparse/command.h"
#include "argparse/memory.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "assert.h"

static void test_version() {
	int out_pipe[2];
	assert(pipe(out_pipe) == 0);

	int saved_stdout = dup(STDOUT_FILENO);

	dup2(out_pipe[1], STDOUT_FILENO);
	close(out_pipe[1]);

	pid_t pid = fork();
	if(pid == 0) {
		struct command command;

		command_init(&command, "program", "0.0.0");

		const int argc = 2;
		char *argv[] = {
			"program",
			"-V"
		};

		command_parse(&command, argc, argv);
		command_destroy(&command);

		exit(EXIT_FAILURE);
	}

	int status;
	waitpid(pid, &status, 0);
	assert(status == 0);

	fflush(stdout);

	char buffer[10];
	read(out_pipe[0], buffer, 10);

	dup2(saved_stdout, STDOUT_FILENO);
	assert(strncmp("0.0.0\n", buffer, 6) == 0);
}

static void test_help() {
	struct command command;

	command_init(&command, "program", "0.0.0");

	const int argc = 2;
	char *argv[] = {
		"program",
		"-h"
	};

	command_parse(&command, argc, argv);
	command_destroy(&command);
}

int main(int argc, char * const *argv) {
	test_version();
	test_help();
	return EXIT_SUCCESS;
}
