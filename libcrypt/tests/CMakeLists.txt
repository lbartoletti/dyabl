#find_package(GTest REQUIRED)
# We need thread support
find_package(Threads REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libcrypt/include)
link_directories(../../libcrypt/lib)
# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libcrypt src/test_crypt.cpp)

target_link_libraries(tests_libcrypt crypt lists ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libcrypt DESTINATION bin)
