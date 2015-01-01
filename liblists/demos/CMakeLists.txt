SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# liblists
file(GLOB DEMO_LIBLISTS_SRCS src/*.c)
include_directories(../libutils/include)
include_directories(../liblists/include)
link_directories(liblists/lib)
link_directories(libutils/lib)

add_executable(demo_liblists ${DEMO_LIBLISTS_SRCS})

target_link_libraries(demo_liblists
    PUBLIC utils
    lists
    )

install(TARGETS demo_liblists DESTINATION bin)
