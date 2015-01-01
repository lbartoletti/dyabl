SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

# libtree
file(GLOB DEMO_LIBTREE_SRCS src/*.c)
include_directories(../libtree/include)
link_directories(libtree/lib)
link_directories(libutils/lib)

add_executable(demo_libtree ${DEMO_LIBTREE_SRCS})

target_link_libraries(demo_libtree
    utils
    tree
    )

install(TARGETS demo_libtree DESTINATION bin)
