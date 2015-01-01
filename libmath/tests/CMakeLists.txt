#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libmath/include)
link_directories(../../libmath/lib)
# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libmath src/test_math.cpp)

target_link_libraries(tests_libmath math lists ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libmath DESTINATION bin)
