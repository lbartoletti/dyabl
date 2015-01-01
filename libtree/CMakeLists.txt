cmake_minimum_required(VERSION 3.8.9)

project(tree)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib/)

include_directories(../libutils/include)
include_directories(../liblists/include)
include_directories(include)

file(
	GLOB_RECURSE
	source_files
	src/*
	)
file(
    GLOB_RECURSE
    include_files
    include/*
    )

add_library(
	tree
	SHARED
	${source_files}
	)
# create static lib
add_library(tree_static STATIC ${source_files}) 

# rewrite static lib
set_target_properties(tree_static PROPERTIES OUTPUT_NAME tree)
if(COVERAGE)
    set_target_properties(tree PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(tree PUBLIC ../libutils/lib ../liblists/lib)
target_link_libraries(tree PUBLIC utils lists)

install(TARGETS tree DESTINATION lib)
install(FILES ${include_files} DESTINATION include)


if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libtree COMMAND tests/bin/tests_libtree)
endif(BUILD_TESTS OR COVERAGE)

