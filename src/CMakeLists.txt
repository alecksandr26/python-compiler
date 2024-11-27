project(${CMAKE_PROJECT_NAME})

# Define the main compiler library
add_library(pyc
  STATIC lexer.cpp tag.cpp token.cpp word.cpp integer.cpp real.cpp ident.cpp parser.cpp ast.cpp error.cpp
  semantic.cpp codegen.cpp
)

# Add the CLI binary executable
add_executable(cli.out
  cli.cpp
)

# Link LLVM and Qt to the `pyc` library
target_link_libraries(pyc
  PUBLIC Qt5::Core
  PRIVATE ${LLVM_LIBRARIES}
)

# Link LLVM and `pyc` to the CLI binary
target_link_libraries(cli.out
  PRIVATE pyc ${LLVM_LIBRARIES}
)

# Set include directories for the `pyc` library
target_include_directories(pyc
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

# Define the GUI library
add_library(pycgui
  STATIC gui.cpp
)

# Link the GUI library with the `pyc` library and Qt5
target_link_libraries(pycgui
  PRIVATE pyc Qt5::Widgets Qt5::Core
)

# Set include directories for the GUI library
target_include_directories(pycgui
  PRIVATE ${Qt5Widgets_INCLUDE_DIRS}
)

# Enable Qt5 specific features for the GUI library
set_target_properties(pycgui PROPERTIES AUTOMOC ON)

# Add the GUI binary executable
add_executable(app.out
  app.cpp
)

# Link the GUI binary with the GUI library and Qt5
target_link_libraries(app.out
  PRIVATE pycgui Qt5::Widgets Qt5::Core)

# Set include directories for the GUI binary
target_include_directories(app.out
  PRIVATE ${SRC_DIR} ${Qt5Widgets_INCLUDE_DIRS})

# Enable Qt5 specific features for the GUI binary
set_target_properties(app.out
  PROPERTIES AUTOMOC ON)
