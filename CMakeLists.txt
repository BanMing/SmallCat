cmake_minimum_required(VERSION 3.0.0)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)

project(GameAnimation VERSION 0.1.0)

add_subdirectory(bgfx.cmake)

file(GLOB_RECURSE ExampleSrc examples/*.h examples/*.cpp)
file(GLOB_RECURSE MathSrc math/*.h math/*.cpp)
file(GLOB_RECURSE AniamtionSrc animation/*.h animation/*.cpp)
file(GLOB_RECURSE GltfSrc gltf/*.h gltf/*.cpp)
file(GLOB_RECURSE GraphicSrc graphic/*.h graphic/*.cpp)

include(test/CMakeLists.txt)

add_executable(GameAnimation main.cpp ${ExampleSrc} ${MathSrc} ${AniamtionSrc} ${GltfSrc})

target_link_libraries(GameAnimation PUBLIC bx bimg bgfx example-common)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT GameAnimation)
if (CMAKE_GENERATOR MATCHES "Visual Studio")
endif()