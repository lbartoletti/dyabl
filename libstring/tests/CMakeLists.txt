#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libstring/include)
link_directories(../../libstring/lib)
# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libstring src/test_string.cpp)

target_link_libraries(tests_libstring string lists ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libstring DESTINATION bin)
