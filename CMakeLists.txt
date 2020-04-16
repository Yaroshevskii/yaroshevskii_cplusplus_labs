cmake_minimum_required(VERSION 3.15)
project(lab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )

find_package(GTest REQUIRED)


add_executable(lab4 main.cpp csvParser.h csvLegacyInputIterator.h)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)