cmake_minimum_required(VERSION 4.0)
project(Proyecto_Final)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES
        src/*.cpp
        src/*/*.cpp
        src/*/*/*.cpp
)

add_executable(Proyecto_Final ${SRC_FILES})
target_include_directories(Proyecto_Final PRIVATE src)
