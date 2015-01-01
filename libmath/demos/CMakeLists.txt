SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libmath
file(GLOB DEMO_LIBMATH_SRCS src/*.c)
include_directories(../libmath/include)
link_directories(libmath/lib)

add_executable(demo_libmath ${DEMO_LIBMATH_SRCS})

target_link_libraries(demo_libmath
    math
    )

install(TARGETS demo_libmath DESTINATION bin)
