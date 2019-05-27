#Dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

include(CTest)

find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --error-exitcode=1")
set(MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/.valgrind-suppressions")

#Tests
add_executable(argparse_tests main.c)
target_link_libraries(argparse_tests PRIVATE argparse)
target_include_directories(argparse_tests PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(argparse_tests PRIVATE ${GLIB_LDFLAGS})
add_test(
	NAME argparse
	COMMAND argparse_tests
)

add_executable(argparse_set_tests set.c)
target_link_libraries(argparse_set_tests PRIVATE argparse)
add_test(
	NAME argparse_set
	COMMAND argparse_set_tests
)

add_executable(argparse_options_tests options.c)
target_link_libraries(argparse_options_tests PRIVATE argparse)
add_test(
	NAME argparse_options
	COMMAND argparse_options_tests
)

add_executable(argparse_subcommands_tests subcommands.c)
target_link_libraries(argparse_subcommands_tests PRIVATE argparse)
add_test(
	NAME argparse_subcommands
	COMMAND argparse_subcommands_tests
)
