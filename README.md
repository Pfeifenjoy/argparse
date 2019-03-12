[![Build Status](https://drone.metthub.de/api/badges/Pfeifenjoy/argparse/status.svg)](https://drone.metthub.de/Pfeifenjoy/argparse)

# Argparse

Argparse is a small library to parse command line arguments.

# Example

The example generates a cli which has the following form:
```

  Usage: custom-flag [options]

  Options:

  -h, --help                            Display help message.
  -V, --version                         Display the version.
  -c, --custom-flag                     A custom flag

```

```c
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
```
