project(${CMAKE_PROJECT_NAME})

add_library(${CMAKE_PROJECT_NAME} SHARED
  lexer.cpp tag.cpp token.cpp word.cpp integer.cpp real.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)






