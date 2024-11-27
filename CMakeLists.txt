cmake_minimum_required(VERSION 3.5)

project(python-compiler VERSION 1.0.0
  DESCRIPTION "python-compiler is just a simple and basic compiler with python syntax"
  LANGUAGES C CXX
)

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable exceptions globally
set(CMAKE_CXX_FLAGS "-fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fexceptions")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fexceptions")

message("-- Initial CXX Compiler Flags: ${CMAKE_CXX_FLAGS}")

# Include GNU Install Directories for standardized install paths
include(GNUInstallDirs)

# Define source and example directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/example")

# Find Qt5 components (Widgets and Core are required)
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# Find LLVM components using llvm-config
find_program(LLVM_CONFIG_EXECUTABLE llvm-config REQUIRED)

if (LLVM_CONFIG_EXECUTABLE)
  # Retrieve LLVM configuration details via llvm-config
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --ldflags
    OUTPUT_VARIABLE LLVM_LD_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --libs core irreader executionengine
    OUTPUT_VARIABLE LLVM_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
    OUTPUT_VARIABLE LLVM_LIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # Filter out `-fno-exceptions` if present in LLVM_CXX_FLAGS
  string(REPLACE "-fno-exceptions" "" LLVM_CXX_FLAGS "${LLVM_CXX_FLAGS}")

  # Apply LLVM configurations
  include_directories(${LLVM_INCLUDE_DIRS})
  link_directories(${LLVM_LIB_DIR})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LD_FLAGS}")
endif()

# Print LLVM configuration details for debugging
message(STATUS "LLVM Libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM Include Dir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM Lib Dir: ${LLVM_LIB_DIR}")

# Add subdirectories for source and examples
add_subdirectory(${SRC_DIR})
add_subdirectory(${EXAMPLE_DIR})
