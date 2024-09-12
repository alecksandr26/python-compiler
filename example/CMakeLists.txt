project(${CMAKE_PROJECT_NAME})

set(MAIN main)

add_executable(${MAIN}
  main.cpp
)

add_executable(example_lexer.out
  example_lexer.cpp
)

target_link_libraries(${MAIN}
  PRIVATE ${CMAKE_PROJECT_NAME}
)


