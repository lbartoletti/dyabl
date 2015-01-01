cmake_minimum_required(VERSION 3.8.9)

project(math)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib/)

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
	math
	SHARED
	${source_files}
	)
# create static lib
add_library(math_static STATIC ${source_files})

# rewrite static lib
set_target_properties(math_static PROPERTIES OUTPUT_NAME math)
if(COVERAGE)
    set_target_properties(math PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS math DESTINATION lib)
install(FILES ${include_files} DESTINATION include)



if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libmath COMMAND tests/bin/tests_libmath)
endif(BUILD_TESTS OR COVERAGE)
