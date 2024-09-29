# Define the project as the parent project's name
project(${CMAKE_PROJECT_NAME})

# Add a library with the specified source files
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


