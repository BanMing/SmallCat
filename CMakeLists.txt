cmake_minimum_required(VERSION 3.0.0)
project(Tiga)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

include(macros)

set( BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries." FORCE)

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DETH_STATIC)
	add_definitions(-DETH_API_EXPORTS)
else()
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
add_definitions(-DNOMINMAX)

#enable_sanitizers("custom")

detect_platform()
print_info()
set_output_paths()

#enable_sanitizers("custom")

# detect_platform()
# print_info()
# set_output_paths()

file(GLOB_RECURSE TigaSrc Src/*.h Src/*.cpp Src/*.hpp Src/*.c Src/*.cc )
foreach(src ${TigaSrc})
	message("Tiga src: [${src}]")
endforeach()

add_executable(Tiga ${TigaSrc})
add_subdirectory_ex(Thirdparty)
set_project_custom_defines()


target_link_libraries( Tiga PUBLIC bgfx bx bimg)