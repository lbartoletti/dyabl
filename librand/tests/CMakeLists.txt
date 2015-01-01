# find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../librand/include)
link_directories(../../librand/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_librand src/test_rand.cpp)

target_link_libraries(tests_librand rand ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_librand DESTINATION bin)
