# Define the project as the parent project's name
project(${CMAKE_PROJECT_NAME})

# Add The pyc library with the specified source files
add_library(pyc
  STATIC lexer.cpp tag.cpp token.cpp word.cpp integer.cpp real.cpp
)

# Set the include directories for the library
target_include_directories(pyc
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

# Here we define the gui librarie, that will contain the compiler but also the gui libraries
add_library(pycgui
  STATIC gui.cpp
)

# Here link the compiler
target_link_libraries(pycgui
  PRIVATE pyc Qt5::Widgets Qt5::Core
)

target_include_directories(pycgui
  PRIVATE ${Qt5Widgets_INCLUDE_DIRS}
)

set_target_properties(pycgui PROPERTIES AUTOMOC ON)

# Adding the gui binary executable


# **Find Qt5 components**
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)


# **Enable Qt-specific features**
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ide.out
  ide.cpp
)

target_link_libraries(ide.out
  PRIVATE pycgui Qt5::Widgets Qt5::Core)

target_include_directories(ide.out
  PRIVATE ${SRC_DIR} ${Qt5Widgets_INCLUDE_DIRS})

set_target_properties(ide.out
  PROPERTIES AUTOMOC ON)
