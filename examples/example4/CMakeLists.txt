cmake_minimum_required(VERSION 3.13)

project(example4)

add_executable(${PROJECT_NAME}ss example4ss.cpp)

target_link_libraries(${PROJECT_NAME}ss pigeon)

add_executable(${PROJECT_NAME}sr example4sr.cpp)

target_link_libraries(${PROJECT_NAME}sr pigeon)