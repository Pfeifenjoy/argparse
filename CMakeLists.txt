cmake_minimum_required(VERSION 3.7)
project(argparse VERSION 0.0.0 LANGUAGES C)

#Dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

#Library
add_library(argparse
	src/command.c
	src/argument.c
	src/arguments.c
	src/option.c
	src/options.c
	src/memory.c
	src/vector.c
)
target_include_directories(argparse PUBLIC include)

#Examples
add_subdirectory(examples)

#Tests
enable_testing()
add_executable(argparse_tests test/main.c)
target_link_libraries(argparse_tests PRIVATE argparse)
target_include_directories(argparse_tests PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(argparse_tests PRIVATE ${GLIB_LDFLAGS})
add_test(
	NAME argparse
	COMMAND argparse_tests
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/test
)

