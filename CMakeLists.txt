cmake_minimum_required(VERSION 3.5)

project(python-compiler VERSION 1.0.0
  DESCRIPTION "python-compiler is just a simple and basic compiler with python syntax"
  LANGUAGES CXX
)

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)

# Define compiler flags
if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -ggdb ")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -ffast-math -march=native")
else()
  # Flags for Windows with MSVC
  set(CMAKE_CXX_FLAGS_DEBUG "/W3 /Od /Zi /DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE EQUAL Release)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

message("-- CXX Compiler Flags: ${CMAKE_CXX_FLAGS}")

include(GNUInstallDirs)

# Define source directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/example")

# **Find Qt5 components using find_package()**
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# Print Qt information
message(STATUS "Qt5Widgets_INCLUDE_DIRS: ${Qt5Widgets_INCLUDE_DIRS}")
message(STATUS "Qt5Widgets_LIBRARIES: ${Qt5Widgets_LIBRARIES}")

# **Enable Qt-specific features**
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# **Now add subdirectories after finding Qt5**
add_subdirectory(${SRC_DIR}/)
add_subdirectory(${EXAMPLE_DIR}/)



