SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libfpn
file(GLOB DEMO_LIBfpn_SRCS src/*.c)
include_directories(../libfpn/include)
link_directories(libfpn/lib)

add_executable(demo_libfpn ${DEMO_LIBfpn_SRCS})

target_link_libraries(demo_libfpn
    fpn
    )

install(TARGETS demo_libfpn DESTINATION bin)
