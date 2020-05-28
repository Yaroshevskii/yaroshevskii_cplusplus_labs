cmake_minimum_required(VERSION 3.16)
project(SolverForRiemmanProblemQT)

set(CMAKE_CXX_STANDARD 17)
set(Qt5_DIR "~/Qt/5.9.5/clang_64/lib/cmake/Qt5")


find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt5Charts)


add_executable(SolverForRiemmanProblemQT main.cpp renderArea.cpp renderArea.h RiemmanSolver.cpp RiemmanSolver.h)
target_link_libraries(SolverForRiemmanProblemQT Qt5::Core Qt5::Widgets Qt5::Charts)

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)