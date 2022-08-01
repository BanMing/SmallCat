
cmake_minimum_required(VERSION 3.1)

if (MSVC AND (MSVC_VERSION GREATER_EQUAL 1914))
    add_compile_options("/Zc:__cplusplus")
endif()

file(GLOB_RECURSE HELLO *.h *.cpp *.hpp *.c *.cc)

add_executable(Hello ${HELLO})

# # Put in a "Samples" folder in Visual Studio
set_target_properties( Hello PROPERTIES FOLDER "Samples" )

target_include_directories( Hello PUBLIC ${BGFX_DIR}/examples/common )
target_include_directories( Hello PUBLIC ${BX_DIR}/include )
target_include_directories( Hello PUBLIC ${BX_DIR}/include/compat/msvc )
target_include_directories( Hello PUBLIC ${BX_DIR}/3rdparty )
target_include_directories( Hello PUBLIC ${BIMG_DIR}/include )
target_include_directories( Hello PUBLIC ${BIMG_DIR}/3rdparty )
target_include_directories( Hello PUBLIC ${BGFX_DIR}/include )
target_include_directories( Hello PUBLIC ${BGFX_DIR}/3rdparty )

