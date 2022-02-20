cmake_minimum_required(VERSION 3.13)

project(example)

aux_source_directory(${CMAKE_SOURCE_DIR} allSource)

add_executable(example ${allSource})

target_link_libraries(example
    PRIVATE pigeon
)