cmake_minimum_required(VERSION 3.10)
project(argparse VERSION 0.0.0 LANGUAGES C)

#Library
add_library(argparse
	src/command.c
	src/arguments.c
	src/optionals.c
	src/memory.c
)
target_include_directories(argparse PUBLIC include)

#Tests
enable_testing()
add_executable(argparse_tests test/main.c)
target_link_libraries(argparse_tests PRIVATE argparse)
add_test(NAME argparse COMMAND argparse_tests)
