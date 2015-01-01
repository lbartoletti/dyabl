# find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

#include_directories(${GTEST_INCLUDE_DIR} ${THREADS_INCLUDE_DIR})
include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libutils/include)
link_directories(../../libutils/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libutils src/test_utils.cpp)

#target_link_libraries(tests_libutils utils ${THREADS_LIBRARY} ${GTEST_LIBRARY} ${GTEST_LIBRARY_MAIN})
target_link_libraries(tests_libutils utils ${THREADS_LIBRARY} gtest gmock_main)

install(TARGETS tests_libutils DESTINATION bin)
