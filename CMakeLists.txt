cmake_minimum_required(VERSION 3.13)

project(codeNamePigeon)

aux_source_directory(${CMAKE_SOURCE_DIR}/src allSource)

add_library(pigeon ${allSource})

target_include_directories(pigeon PUBLIC ${CMAKE_SOURCE_DIR}/include)


#enable higher c++ standard
target_compile_options(pigeon PUBLIC $<$<COMPILE_LANGUAGE:CXX>: -Wall -Wextra -std=c++17 -static
 -g
>)

#link to the thread library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(
	pigeon
	PRIVATE Threads::Threads
)

add_executable(c1 c1.cpp)

target_link_libraries(c1
	PRIVATE pigeon
	PRIVATE Threads::Threads
)

add_executable(s1 s1.cpp)

target_link_libraries(s1
	PRIVATE pigeon
	PRIVATE Threads::Threads
)


add_executable(bc1 bc1.cpp)

target_link_libraries(bc1
	PRIVATE pigeon
	PRIVATE Threads::Threads
)

add_executable(bs1 bs1.cpp)

target_link_libraries(bs1
	PRIVATE pigeon
	PRIVATE Threads::Threads
)

# add_executable(test3c test3c.cpp)

# target_link_libraries(test3c
# 	PRIVATE pigeon
# 	PRIVATE Threads::Threads
# )


# add_executable(test3s test3s.cpp)

# target_link_libraries(test3s
# 	PRIVATE pigeon
# 	PRIVATE Threads::Threads
# )

# add_executable(test4s test4s.cpp)

# target_link_libraries(test4s
# 	PRIVATE pigeon
# 	PRIVATE Threads::Threads
# )

