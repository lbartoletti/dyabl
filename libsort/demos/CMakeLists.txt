SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libsort
file(GLOB DEMO_LIBSORT_SRCS src/*.c)
include_directories(../libsort/include)
link_directories(libsort/lib)

add_executable(demo_libsort ${DEMO_LIBSORT_SRCS})

target_link_libraries(demo_libsort
    sort
    )

install(TARGETS demo_libsort DESTINATION bin)
