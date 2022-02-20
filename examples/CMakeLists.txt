cmake_minimum_required(VERSION 3.13)

project(example)

aux_source_directory(${CMAKE_SOURCE_DIR} allSource)
message(STATUS ${allSource})

foreach(exampleSource ${allSource})
    #chop-off ".cpp"
    string( REPLACE ".cpp" "" example_i ${exampleSource} )
    #chop-off the trailing path
    string( REPLACE "${CMAKE_SOURCE_DIR}/" "" example ${example_i} )
    add_executable( ${example} ${exampleSource} )

    target_link_libraries(${example}
        PRIVATE pigeon
    )  
endforeach(exampleSource ${allSource})

