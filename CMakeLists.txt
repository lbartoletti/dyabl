cmake_minimum_required(VERSION 3.8.9)

# Nom du projet
project(dyabl C CXX)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    # using AppleClang or Clang
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Weverything -std=c11")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)
    if(CMAKE_USE_PTHREADS_INIT)
        set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
    endif()
  # using GCC
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c11 -Wall -Wextra")
  #  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror-implicit-function-declaration -Wmain -Wparentheses")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconversion -Wsign-compare -Waggregate-return -Wstrict-prototypes")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long")
  #SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunreachable-code")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c11")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  # using Intel C++
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c11")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  # using Visual Studio C++
endif()

option(DEBUG "DEBUG" ON)
option(BUILD_DEMOS "Build demo programs" ON)
option(BUILD_DOCS "Create and install the HTML based API documentation (requires Doxygen)" ON)
option(BUILD_TESTS "Build test programs" ON)
option(COVERAGE "Code coverage" ON)
option(DOWNLOAD_GTEST "Download googletest" ON)

if(DEBUG)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
endif(DEBUG)

if(BUILD_DOCS)
    # add a target to generate API documentation with Doxygen
    find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif(NOT DOXYGEN_FOUND)

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html/ DESTINATION doc)
endif(BUILD_DOCS)

if(BUILD_TESTS OR COVERAGE)
    include(CTest)

    if(DOWNLOAD_GTEST)
      if (CMAKE_VERSION VERSION_LESS 3.2)
          set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
      else()
          set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
      endif()

      include(DownloadProject.cmake)
      download_project(PROJ                googletest
                      GIT_REPOSITORY      https://github.com/google/googletest.git
                      GIT_TAG             master
                      ${UPDATE_DISCONNECTED_IF_AVAILABLE}
      )

      # Prevent GoogleTest from overriding our compiler/linker options
      # when building with Visual Studio
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

      add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

      # When using CMake 2.8.11 or later, header path dependencies
      # are automatically added to the gtest and gmock targets.
      # For earlier CMake versions, we have to explicitly add the
      # required directories to the header search path ourselves.
      if (CMAKE_VERSION VERSION_LESS 2.8.11)
          include_directories("${gtest_SOURCE_DIR}/include"
                              "${gmock_SOURCE_DIR}/include")
      endif()
    else()
	    find_package(GTest REQUIRED)
	    include_directories(${GTEST_INCLUDE_DIRS})
      link_directories("${CMAKE_INSTALL_PREFIX}/lib")
    endif()

    if(COVERAGE)
#        SET(CMAKE_C_COMPILER "/usr/local/bin/clang37")
#        SET(CMAKE_CXX_COMPILER "/usr/local/bin/clang++37")
        add_custom_target(coverage COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_coverage.sh ${CMAKE_CURRENT_SOURCE_DIR})
    endif(COVERAGE)
endif(BUILD_TESTS OR COVERAGE)

add_subdirectory(libarray)
add_subdirectory(libfpn)
add_subdirectory(libhash)
add_subdirectory(libgraph)
add_subdirectory(liblists)
add_subdirectory(libmath)
add_subdirectory(librand)
add_subdirectory(libsort)
add_subdirectory(libstring)
add_subdirectory(libtree)
add_subdirectory(libutils)
add_subdirectory(libcrypt)
