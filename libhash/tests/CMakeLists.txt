#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libhash/include)
include_directories(../../liblists/include)
link_directories(../../libhash/lib)
link_directories(../../liblists/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libhash src/test_hash.cpp)

target_link_libraries(tests_libhash hash ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libhash DESTINATION bin)
