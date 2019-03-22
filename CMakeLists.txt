cmake_minimum_required(VERSION 3.7)
project(Argparse LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(require-git)

#Library
add_library(Argparse
	src/command.c
	src/argument.c
	src/arguments.c
	src/option.c
	src/options.c
	src/memory.c
	src/vector.c
	src/set.c
	src/context.c
)
target_include_directories(Argparse PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	enable_testing()
	add_subdirectory(examples)
	add_subdirectory(test)
endif()


#Install
install(TARGETS Argparse EXPORT ArgparseTargets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY include/argparse DESTINATION include)

install(EXPORT ArgparseTargets
	FILE ArgparseTargets.cmake
	NAMESPACE Argparse::
	DESTINATION lib/cmake/Argparse
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("ArgparseConfigVersion.cmake"
	VERSION ${Argparse_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(
	FILES
		"cmake/ArgparseConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/ArgparseConfigVersion.cmake"
	DESTINATION
		lib/cmake/Argparse
)

include(pack)
