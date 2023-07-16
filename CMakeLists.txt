cmake_minimum_required (VERSION 3.11)
project (Omloppsvektor)

set (CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Eigen3 REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
add_executable (Omloppsvektor src/main.cpp src/body.cpp src/graphics.cpp src/simulation.cpp)
target_link_libraries(Omloppsvektor glfw OpenGL GLEW yaml-cpp Eigen3::Eigen)
