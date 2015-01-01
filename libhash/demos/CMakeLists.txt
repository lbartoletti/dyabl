SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libhash
file(GLOB DEMO_LIBHASH_SRCS src/*.c)
include_directories(../libhash/include)
link_directories(libhash/lib)

add_executable(demo_libhash ${DEMO_LIBHASH_SRCS})

target_link_libraries(demo_libhash
    hash
    )

install(TARGETS demo_libhash DESTINATION bin)
