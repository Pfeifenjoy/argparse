cmake_minimum_required(VERSION 3.5)
project(argparse LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(require-git)

#Library
add_library(argparse
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
target_include_directories(argparse PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	enable_testing()
	add_subdirectory(examples)
	add_subdirectory(test)
endif()


#Install
install(TARGETS argparse EXPORT argparseTargets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY include/argparse DESTINATION include)

install(EXPORT argparseTargets
	FILE argparseTargets.cmake
	NAMESPACE argparse::
	DESTINATION lib/cmake/argparse
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("argparseConfigVersion.cmake"
	VERSION ${argparse_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(
	FILES
		"cmake/argparseConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/argparseConfigVersion.cmake"
	DESTINATION
		lib/cmake/argparse
)

include(pack)
