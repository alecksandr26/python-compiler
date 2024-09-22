# Define the project as the parent project's name
project(${CMAKE_PROJECT_NAME})

# Add a library with the specified source files
add_library(pyc SHARED
  lexer.cpp
  tag.cpp
  token.cpp
  word.cpp
  integer.cpp
  real.cpp
  lexer_error.cpp
  MainWindow.cpp
)

# Set the include directories for the library
target_include_directories(pyc
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}"
  PRIVATE ${Qt5Widgets_INCLUDE_DIRS}
)

# Link the Qt5 Widgets library to the 'pyc' library
target_link_libraries(pyc PRIVATE Qt5::Widgets Qt5::Core)

# Enable AUTOMOC for the 'pyc' target
set_target_properties(pyc PROPERTIES AUTOMOC ON)
