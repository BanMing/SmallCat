cmake_minimum_required(VERSION 3.0.0)
# Enable C++11
set(CMAKE_CXX_STANDARD 11)

project(GameAnimation VERSION 0.1.0)

file(GLOB_RECURSE ExampleSrc examples/*.h examples/*.cpp)
file(GLOB_RECURSE MathSrc math/*.h math/*.cpp)

include(test/CMakeLists.txt)

add_executable(GameAnimation main.cpp ${ExampleSrc} ${MathSrc})

add_subdirectory(bgfx.cmake)

target_link_libraries(GameAnimation PUBLIC bx bimg bgfx example-common)
