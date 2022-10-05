cmake_minimum_required(VERSION 3.0.0)
project(GameAnimation VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE ExampleSrc examples/*.h examples/*.cpp)
file(GLOB_RECURSE MathSrc math/*.h math/*.cpp)

add_executable(GameAnimation main.cpp ${ExampleSrc} ${MathSrc})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(bgfx.cmake)

target_link_libraries(GameAnimation PUBLIC bx bimg bgfx example-common)
