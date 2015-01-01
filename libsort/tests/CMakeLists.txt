#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libsort/include)
link_directories(../../libsort/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libsort src/test_sort.cpp)

target_link_libraries(tests_libsort sort ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libsort DESTINATION bin)
