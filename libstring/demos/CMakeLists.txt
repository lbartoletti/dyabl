SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libstring
file(GLOB DEMO_LIBSTRING_SRCS src/*.c)
include_directories(../libstring/include)
include_directories(../libutils/include)
link_directories(libstring/lib)

add_executable(demo_libstring ${DEMO_LIBSTRING_SRCS})

target_link_libraries(demo_libstring
    string
    )

install(TARGETS demo_libstring DESTINATION bin)
