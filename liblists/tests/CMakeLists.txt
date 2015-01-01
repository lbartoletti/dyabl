# find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(include)
include_directories(../../liblists/include)
include_directories(../../libutils/include)
link_directories(../../liblists/lib)
link_directories(../../libutils/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB TESTS_LIBLISTS_SRCS src/*.cpp)
add_executable(tests_liblists ${TESTS_LIBLISTS_SRCS})

target_link_libraries(tests_liblists lists utils ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_liblists DESTINATION bin)
