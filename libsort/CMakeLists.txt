cmake_minimum_required(VERSION 3.8.9)

project(sort)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib/)

include_directories(../libutils/include)
include_directories(../libarray/include)
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
	sort
	SHARED
	${source_files}
	)
# create static lib
add_library(sort_static STATIC ${source_files}) 

# rewrite static lib
set_target_properties(sort_static PROPERTIES OUTPUT_NAME sort)
if(COVERAGE)
    set_target_properties(sort PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(sort PUBLIC ../libutils/lib ../libarray/lib)
target_link_libraries(sort PUBLIC utils array)

install(TARGETS sort DESTINATION lib)
install(FILES ${include_files} DESTINATION include)


if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libsort COMMAND tests/bin/tests_libsort)
endif(BUILD_TESTS OR COVERAGE)

