cmake_minimum_required(VERSION 3.8.9)

project(lists)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib/)

include_directories(../libutils/include)
include_directories(include)

file(
	GLOB_RECURSE
	source_files
	src/*
	)
add_library(
	lists
	SHARED
	${source_files}
	)
# create static lib
add_library(lists_static STATIC ${source_files})

# rewrite static lib
set_target_properties(lists_static PROPERTIES OUTPUT_NAME lists)
if(COVERAGE)
    set_target_properties(lists PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(lists PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lists PUBLIC ../libutils/include)

install(TARGETS lists DESTINATION lib)
install(FILES ${include_files} DESTINATION include)



if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME liblists COMMAND tests/bin/tests_liblists)
endif(BUILD_TESTS OR COVERAGE)
