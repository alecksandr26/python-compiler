project(${CMAKE_PROJECT_NAME})

set(MAIN main)

add_executable(${MAIN}
  main.cpp
)

target_link_libraries(${MAIN}
  ${CMAKE_PROJECT_NAME}
)


