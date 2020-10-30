cmake_minimum_required(VERSION 3.17)
project(RNK)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )

add_executable(RNK main.cpp mThread.cpp mThread.h main.h)