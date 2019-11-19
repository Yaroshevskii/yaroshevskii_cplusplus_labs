cmake_minimum_required(VERSION 3.14)
project(task0)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

add_executable(task0 main.cpp mLife.cpp mLife.h mUniverse.cpp mUniverse.h)