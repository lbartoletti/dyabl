SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libgraph
file(GLOB DEMO_LIBGRAPH_SRCS src/*.c)
include_directories(../libgraph/include)
link_directories(libgraph/lib)

add_executable(demo_libgraph ${DEMO_LIBGRAPH_SRCS})

target_link_libraries(demo_libgraph
    graph
    )

install(TARGETS demo_libgraph DESTINATION bin)
