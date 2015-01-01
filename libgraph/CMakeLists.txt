cmake_minimum_required(VERSION 3.8.9)

project(graph)

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
        graph
	SHARED
	${source_files}
	)
# create static lib
add_library(graph_static STATIC ${source_files})

# rewrite static lib
set_target_properties(graph_static PROPERTIES OUTPUT_NAME graph)
if(COVERAGE)
    set_target_properties(graph PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(graph PUBLIC ../libutils/lib ../liblists/lib)
target_link_libraries(graph PUBLIC utils lists)

install(TARGETS graph DESTINATION lib)
install(FILES ${include_files} DESTINATION include)



if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libgraph COMMAND tests/bin/tests_libgraph)
endif(BUILD_TESTS OR COVERAGE)
