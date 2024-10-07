cmake_minimum_required(VERSION 3.5)

project(python_compiler VERSION 1.0.0
  DESCRIPTION "python-compiler is just a simple and basic compiler with python syntax"
  LANGUAGES CXX
)

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define compiler flags
if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -ggdb")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -ffast-math -march=native")
endif()

# Set the build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message("-- Build Type: ${CMAKE_BUILD_TYPE}")

# Include GNUInstallDirs
include(GNUInstallDirs)

# Define source directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Find Qt5 components
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# Print Qt information
message(STATUS "Qt5Widgets_INCLUDE_DIRS: ${Qt5Widgets_INCLUDE_DIRS}")
message(STATUS "Qt5Widgets_LIBRARIES: ${Qt5Widgets_LIBRARIES}")

# Enable Qt-specific features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add subdirectories
add_subdirectory(src)
