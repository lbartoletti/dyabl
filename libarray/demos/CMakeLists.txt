SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libarray
file(GLOB DEMO_LIBARRAY_SRCS src/*.c)
include_directories(../libutils/include)
include_directories(../libarray/include)
link_directories(libarray/lib)

add_executable(demo_libarray ${DEMO_LIBARRAY_SRCS})

target_link_libraries(demo_libarray
    array
    )

install(TARGETS demo_libarray DESTINATION bin)
