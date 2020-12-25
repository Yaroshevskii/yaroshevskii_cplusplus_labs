cmake_minimum_required(VERSION 3.17)
project(mHashMap)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )

add_executable(mHashMap main.cpp schedulePresenter.cpp schedulePresenter.h mIter.h mHM.cpp mHM.h)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)