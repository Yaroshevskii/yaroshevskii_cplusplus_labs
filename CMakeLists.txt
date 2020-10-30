cmake_minimum_required(VERSION 3.17)
project(RNK)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp main.h)


find_package(GTest REQUIRED)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)