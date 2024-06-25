cmake_minimum_required(VERSION 3.16)

project(cube)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#add_compile_options(-fsanitize=address)
#add_compile_options(-g)
#add_link_options(-fsanitize=address)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /w14640")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
endif()

if(NOT TARGET cube)
    SET(SOURCE
        src/main.cpp
    )
    add_executable(cube ${SOURCE})
endif()

if(NOT TARGET cube-test)
    SET(SOURCE
        test/test.cpp
    )
    add_executable(cube-test ${SOURCE})
    add_test(NAME cube-test COMMAND cube-test)
endif()

