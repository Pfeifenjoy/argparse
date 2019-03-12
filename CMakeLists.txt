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
target_include_directories(argparse PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

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

#Install
install(TARGETS argparse EXPORT Targets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY include/argparse DESTINATION include)

install(EXPORT Targets
	FILE argparseTargets.cmake
	NAMESPACE argparse::
	DESTINATION lib/cmake/argparse
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(argparseConfigVersion.cmake
	COMPATIBILITY SameMajorVersion
)

install(FILES build/argparseConfigVersion.cmake
	DESTINATION lib/cmake/argparse
)

#CPack
set(CPACK_PACKAGE_CONTACT "arwed.mett@googlemail.com")
set(CPACK_PACKAGE_NAME "argparse")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
if(APPLE)
	set(CPACK_GENERATOR "TGZ;ZIP;productbuild")
elseif(UNIX)
	set(CPACK_GENERATOR "TGZ;ZIP")
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP;NSIS;NSIS64")
else()
	set(CPACK_GENERATOR "TGZ;ZIP")
endif()

set(CPACK_BUNDLE_NAME argparse)
include(CPack)
