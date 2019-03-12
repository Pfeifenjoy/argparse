#include "argparse/argparse.h"

void custom_flag_set(context_t *context) {
	//custom flag was set.
	bool *flag_set = context->data;
	*flag_set = true;
}

int main(int argc, const char **argv) {
	command_t command;
	command_init(&command, "custom-flag", "0.0.0");

	//short name, long-name, description, callback
	//-> -c, --custom-flag, A custom flag
	command_flag(&command, 'c', "custom-flag", "A custom flag", custom_flag_set);

	bool flag_set = false; //this will be passed to the context
	command_parse(&command, &flag_set, argc, argv);

	command_destroy(&command);
}
