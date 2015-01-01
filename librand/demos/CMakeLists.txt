SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# librand
file(GLOB DEMO_LIBRAND_SRCS src/*.c)
include_directories(../librand/include)
link_directories(librand/lib)

add_executable(demo_librand ${DEMO_LIBRAND_SRCS})

target_link_libraries(demo_librand
    rand
    )

install(TARGETS demo_librand DESTINATION bin)
