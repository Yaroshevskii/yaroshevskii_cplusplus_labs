cmake_minimum_required(VERSION 3.15)
project(qtTest)

set(CMAKE_CXX_STANDARD 17)
set(Qt5_DIR "~/Qt/5.9.5/clang_64/lib/cmake/Qt5")


find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt5Charts)


add_executable(qtTest main.cpp renderArea.cpp renderArea.h)
target_link_libraries(qtTest Qt5::Core Qt5::Widgets Qt5::Charts)


target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)