cmake_minimum_required(VERSION 3.16)
project(FinalChecking)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )

find_package(GTest REQUIRED)

add_executable(FinalChecking main.cpp mWeakPtr.h)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)