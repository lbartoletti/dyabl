SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libcrypt
file(GLOB DEMO_LIBCRYPT_SRCS src/*.c)
include_directories(../libcrypt/include)
link_directories(libcrypt/lib)

add_executable(demo_libcrypt ${DEMO_LIBCRYPT_SRCS})

target_link_libraries(demo_libcrypt
    crypt
    )

install(TARGETS demo_libcrypt DESTINATION bin)
