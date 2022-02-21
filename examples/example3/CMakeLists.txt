cmake_minimum_required(VERSION 3.13)

project(example3)

add_executable(${PROJECT_NAME}c example3c.cpp)

target_link_libraries(${PROJECT_NAME}c pigeon)

add_executable(${PROJECT_NAME}s example3s.cpp)

target_link_libraries(${PROJECT_NAME}s pigeon)