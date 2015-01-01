#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libtree/include)
include_directories(../../libarray/include)
link_directories(../../libtree/lib)
link_directories(../../libarray/lib)
link_directories(../../libutils/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libtree src/test_tree.cpp)

target_link_libraries(tests_libtree tree array utils ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libtree DESTINATION bin)
