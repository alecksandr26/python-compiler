# example/CMakeLists.txt

project(${CMAKE_PROJECT_NAME})

# Set up the main executable name
set(MAIN main)

# **Find Qt5 components**
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)


# **Enable Qt-specific features**
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# TODO: We need to define constant variables to name all the executables etc

# Define binaries for the project
add_executable(gui_app.out
  ${EXAMPLE_DIR}/gui_app.cpp
)

# Link the QtApp executable to the Qt5 libraries and the pyc library
target_link_libraries(gui_app.out
  PRIVATE pycgui Qt5::Widgets Qt5::Core
)

target_include_directories(gui_app.out
  PRIVATE ${SRC_DIR} ${Qt5Widgets_INCLUDE_DIRS}
)

set_target_properties(gui_app.out PROPERTIES AUTOMOC ON)
