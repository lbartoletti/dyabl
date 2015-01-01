cmake_minimum_required(VERSION 3.8.9)

project(string)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib/)

include_directories(../libutils/include)
include_directories(include)
file(
	GLOB_RECURSE
	source_files
	src/*
	)

# this is the "object library" target: compiles the sources only once
add_library(
	string
	SHARED
	${source_files}
	)
# create static lib
add_library(string_static STATIC ${source_files})

# rewrite static lib
set_target_properties(string_static PROPERTIES OUTPUT_NAME string)
if(COVERAGE)
    set_target_properties(string PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(string PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(string PUBLIC ../libutils/include)

install(TARGETS string DESTINATION lib)
install(FILES String.h DESTINATION include)



if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libstring COMMAND tests/bin/tests_libstring)
endif(BUILD_TESTS OR COVERAGE)


