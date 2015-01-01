cmake_minimum_required(VERSION 3.8.9)

project(utils)

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
	utils
	SHARED
	${source_files}
	)
# create static lib
add_library(utils_static STATIC ${source_files})

# rewrite static lib
set_target_properties(utils_static PROPERTIES OUTPUT_NAME utils)
if(COVERAGE)
    set_target_properties(utils PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs"
                                        LINK_FLAGS "-fprofile-arcs")
endif()

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS utils DESTINATION lib)
install(FILES ${include_files} DESTINATION include)



if(BUILD_DEMOS)
    add_subdirectory(demos)
endif(BUILD_DEMOS)

if(BUILD_TESTS OR COVERAGE)
    add_subdirectory(tests)
    add_test(NAME libutils COMMAND tests/bin/tests_libutils)
endif(BUILD_TESTS OR COVERAGE)
