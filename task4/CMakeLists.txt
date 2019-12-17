cmake_minimum_required(VERSION 3.14)
project(task2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

add_executable(task2 main.cpp HuffmanNode.h HuffmanNode.cpp)