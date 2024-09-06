project(${CMAKE_PROJECT_NAME})


add_library(${CMAKE_PROJECT_NAME} SHARED
  lexer.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)






