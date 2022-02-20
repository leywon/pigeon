cmake_minimum_required(VERSION 3.13)

project(pigeon
	LANGUAGES CXX
	VERSION 0.0.1
)

#collect all source codes into a list called ``allSource''
aux_source_directory(${CMAKE_SOURCE_DIR}/src allSource)


#add source code to project
add_library(pigeon ${allSource})

#specifying the include dir
target_include_directories(pigeon PUBLIC ${CMAKE_SOURCE_DIR}/include)


#tune the compiling options
target_compile_options(pigeon PUBLIC $<$<COMPILE_LANGUAGE:CXX>: -Wall -Wextra -std=c++17 -static>)

#link to the thread library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(
	pigeon
	PRIVATE Threads::Threads
)

#This module contains the information about
#where to install the static library
include(GNUInstallDirs)

#install the main library
install(
    TARGETS pigeon
    #"ARCHIVE" means the static library
    ARCHIVE
    	DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    	COMPONENT "core"
)

#install the header
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	FILES_MATCHING PATTERN "*.hpp"
)

#install copyright info
install(
	DIRECTORY ${CMAKE_SOURCE_DIR}/others/
	DESTINATION ${CMAKE_INSTALL_DATADIR}/docs/${PROJECT_NAME}
)

set(CPACK_PACKAGE_DESCRIPTION
    "A simple C++ Unix Socket Netowork library: enable convenient implementations of TCP and UDP socket programming."
)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Simple C++ Unix Socket Netowork library"
)
#generator for generating a ".deb" package
set(CPACK_GENERATOR "DEB")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "wang (cuboidtrick, warren, leywon, etc)")
set(CPACK_PACKAGE_CONTACT "warrenleywon@gmail.com")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS YES)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(
    CPACK_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
include(CPack)
