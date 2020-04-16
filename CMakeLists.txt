cmake_minimum_required(VERSION 3.15)
project(Integrator_lab)

set(CMAKE_CXX_STANDARD 17)
find_package(GTest REQUIRED)

add_executable(Integrator_lab main.cpp prototype.h)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)