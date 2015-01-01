SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libutils
file(GLOB DEMO_LIButils_SRCS src/*.c)
include_directories(../libutils/include)
link_directories(libutils/lib)

add_executable(demo_libutils ${DEMO_LIButils_SRCS})

target_link_libraries(demo_libutils
    utils
    )

install(TARGETS demo_libutils DESTINATION bin)
