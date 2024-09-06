# To build the build system debug purpuse
# cmake . -B build/

# To build the project for release version
# cmake . -B build/ -DCMAKE_BUILD_TYPE=Release

# To build the project with a local install directory
# cmake . -B build/ -DCMAKE_INSTALL_PREFIX=./install/

# To compile the project:
# `cd build/ && make `
# or,
# `make -C build/`

cmake_minimum_required(VERSION 3.5)


project(python-compiler VERSION 1.0.0
  DESCRIPTION "python-compiler is just a simple and basic compiler with python syntax"
  LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -ggdb")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -ffast-math -march=native")
else()
  # Flags for Windows with MSVC
  set(CMAKE_CXX_FLAGS_DEBUG "/W3 /Od /Zi /DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE EQUAL Release)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE})
else()
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
endif()

message("-- CXX Compiler Flags: ${CMAKE_CXX_FLAGS}")

include(GNUInstallDirs)


set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXAMPLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/example)

add_subdirectory(${SRC_DIR}/)
add_subdirectory(${EXAMPLE_DIR})




