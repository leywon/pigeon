cmake_minimum_required(VERSION 3.13)

project(pigeon
	LANGUAGES CXX
	VERSION 0.1.5
)

include(GNUInstallDirs)

#link to the thread library
find_package(Threads)

#collect all source codes into a list called ``allSource''
aux_source_directory(${pigeon_SOURCE_DIR}/src allSource)


#add source code to project
add_library(pigeon STATIC ${allSource})

#specifying the include dir
target_include_directories(pigeon 
                           PUBLIC "$<BUILD_INTERFACE:${pigeon_SOURCE_DIR}/include>"
                           INTERFACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/pigeon>"
                           )


target_link_libraries(
	pigeon
	PRIVATE ${CMAKE_THREAD_LIBS_INIT}
)

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_options(-O3 -match=native)

    # install the header
    install(DIRECTORY ${pigeon_SOURCE_DIR}/include/ 
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pigeon)

    install(TARGETS pigeon
                EXPORT pigeonTarget
                ARCHIVE
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}
            )
    
    install(EXPORT pigeonTarget
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pigeon)

    install(FILES "pigeonConfig.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pigeon)

else()
    target_compile_options(pigeon PUBLIC $<$<COMPILE_LANGUAGE:CXX>: -Wall -Wextra -std=c++17 -static>)
    add_subdirectory(examples)
endif()