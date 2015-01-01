if (NOT DOWNLOAD_GTEST)
	find_package(GTest REQUIRED)
endif()
# We need thread support
find_package(Threads REQUIRED)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

include_directories(${THREADS_INCLUDE_DIR})

include_directories(../../libarray/include)
include_directories(../../libutils/include)
link_directories(../../libarray/lib)

# Need pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tests_libarray src/test_array.cpp)

target_link_libraries(tests_libarray array ${THREADS_LIBRARY} gtest gmock_main)
install(TARGETS tests_libarray DESTINATION bin)
