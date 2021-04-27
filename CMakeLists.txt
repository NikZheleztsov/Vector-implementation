cmake_minimum_required(VERSION 3.20.1)

project(vector)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES src/main.cpp src/vector.cpp src/templ.cpp)

add_executable(vector ${SOURCES})
target_include_directories (vector PUBLIC include)
