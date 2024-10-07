# Define the pyc library
add_library(pyc
  STATIC lexer.cpp tag.cpp token.cpp word.cpp integer.cpp real.cpp parser.cpp
)

target_include_directories(pyc
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

# Define the pycgui library
add_library(pycgui
  STATIC gui.cpp
)

target_include_directories(pycgui
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

# Link pycgui with pyc and Qt5 libraries
target_link_libraries(pycgui
  PRIVATE pyc Qt5::Widgets Qt5::Core
)

# Set properties for pycgui
set_target_properties(pycgui PROPERTIES AUTOMOC ON)

# Define the executable for the GUI application
add_executable(app.out
  app.cpp
)

# Link the executable with pycgui, pyc, and Qt5
target_link_libraries(app.out
  PRIVATE pycgui pyc Qt5::Widgets Qt5::Core
)

# Set properties for app.out
set_target_properties(app.out PROPERTIES AUTOMOC ON)
