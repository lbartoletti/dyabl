# find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libfpn/include)
link_directories(../../libfpn/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
add_executable(tests_libfpn src/test_fpn.cpp)

target_link_libraries(tests_libfpn fpn ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libfpn DESTINATION bin)
