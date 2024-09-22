# example/CMakeLists.txt

project(${CMAKE_PROJECT_NAME})

# Set up the main executable name
set(MAIN main)

# **Find Qt5 components**
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# **Include Qt5 directories**
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# **Enable Qt-specific features**
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# **Add the main executable**
add_executable(${MAIN}
  main.cpp
)

# **Link the main executable with pyc library and Qt5Widgets**
target_link_libraries(${MAIN}
  PRIVATE pyc Qt5::Widgets Qt5::Core
)
