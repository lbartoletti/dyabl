cmake_minimum_required(VERSION 3.8.9)

project(array)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib/)

include_directories(../libutils/include)
include_directories(../librand/include)
include_directories(include)
file(
	GLOB_RECURSE
	source_files
	src/*
	)

# this is the "object library" target: compiles the sources only once
add_library(
	array
	SHARED
	${source_files}
	)
# create static lib
add_library(array_static STATIC ${source_files})

# rewrite static lib
set_target_properties(array_static PROPERTIES OUTPUT_NAME array)

if(COVERAGE)
    set_target_properties(array PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()
target_include_directories(array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(array PUBLIC ../libutils/lib)
target_include_directories(array PUBLIC ../librand/lib)
target_link_libraries(array PUBLIC utils rand)

install(TARGETS array DESTINATION lib)
install(FILES Array.h DESTINATION include)



if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libarray COMMAND tests/bin/tests_libarray)
endif(BUILD_TESTS OR COVERAGE)


