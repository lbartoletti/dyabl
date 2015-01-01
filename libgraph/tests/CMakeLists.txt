#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libgraph/include)
link_directories(../../libgraph/lib)
# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libgraph src/test_graph.cpp)

target_link_libraries(tests_libgraph graph lists ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libgraph DESTINATION bin)
