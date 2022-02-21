cmake_minimum_required(VERSION 3.13)

project(example5)

add_executable(${PROJECT_NAME} example5.cpp)

target_link_libraries(${PROJECT_NAME} pigeon)
