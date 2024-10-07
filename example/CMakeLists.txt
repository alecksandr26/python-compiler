# Define the project (optional)
# project(python_compiler_app)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add the executable
add_executable(app.out
  main.cpp
)

# Link the executable with pycgui and Qt5 libraries
target_link_libraries(app.out
  PRIVATE pycgui Qt5::Widgets Qt5::Core
)

# Set properties for app.out
set_target_properties(app.out
  PROPERTIES AUTOMOC ON)
