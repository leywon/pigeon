cmake_minimum_required(VERSION 3.13)

project(example1)

add_executable(${PROJECT_NAME} example1.cpp)

target_link_libraries(${PROJECT_NAME} pigeon)