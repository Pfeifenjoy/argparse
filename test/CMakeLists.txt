#Dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

#Tests
add_executable(argparse_tests main.c)
target_link_libraries(argparse_tests PRIVATE Argparse)
target_include_directories(argparse_tests PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(argparse_tests PRIVATE ${GLIB_LDFLAGS})
add_test(
	NAME argparse
	COMMAND argparse_tests
)

add_executable(argparse_set_tests set.c)
target_link_libraries(argparse_set_tests PRIVATE Argparse)
add_test(
	NAME argparse_set
	COMMAND argparse_set_tests
)
