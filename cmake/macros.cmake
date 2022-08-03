macro(add_subdirectory_ex arg)
    message(STATUS "ADD SUBDIRECTORY [${arg}] START")
    add_subdirectory(${arg})
    message(STATUS "ADD SUBDIRECTORY [${arg}] END")
endmacro(add_subdirectory_ex arg)

macro(detect_platform)
    if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
    endif(UNIX AND NOT APPLE)
    
    # if(NOT LINUX) should work, too, if you need that
    if(LINUX)
        message(STATUS "OS >>> Linux")
    elseif(WIN32)
        message(STATUS "OS >>> Windows")
    endif(LINUX)

    message(STATUS "COMPILER >>> ${CMAKE_CXX_COMPILER_ID}")
   
    message(STATUS "BUILD_SHARED_LIBS >>> ${BUILD_SHARED_LIBS}")

    # platform commond requirement
    if( BGFX_WITH_GLFW )
			find_package( glfw3 REQUIRED )
			target_link_libraries( ${CMAKE_PROJECT_NAME} PUBLIC glfw )
			target_compile_definitions( ${CMAKE_PROJECT_NAME} PUBLIC ENTRY_CONFIG_USE_GLFW )
	elseif( BGFX_WITH_SDL )
		find_package( SDL2 REQUIRED )
		target_link_libraries( ${CMAKE_PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} )
		target_compile_definitions( ${CMAKE_PROJECT_NAME} PUBLIC ENTRY_CONFIG_USE_SDL )
	elseif( UNIX AND NOT APPLE )
		target_link_libraries( ${CMAKE_PROJECT_NAME} PUBLIC X11 )
	endif()

    # platform properties
    if( MSVC )
		set_target_properties( ${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:\"mainCRTStartup\"" )
	endif()

	if( IOS )
		set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE ON
												  MACOSX_BUNDLE_GUI_IDENTIFIER ${CMAKE_PROJECT_NAME}
												  MACOSX_BUNDLE_BUNDLE_VERSION 0
												  MACOSX_BUNDLE_SHORT_VERSION_STRING 0
												  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	endif()

    if (EMSCRIPTEN)
		target_link_libraries(${CMAKE_PROJECT_NAME}
			"-s PRECISE_F32=1"
			"-s TOTAL_MEMORY=268435456"
			"--memory-init-file 1")
	endif()
endmacro(detect_platform)

macro(print_info)
    message(STATUS "PROJECT_BINARTY_DIR >>> ${PROJECT_BINARY_DIR}")
	message(STATUS "CMAKE_CXX_FLAGS >>> ${CMAKE_CXX_FLAGS}")
	message(STATUS "CMAKE_CXX_FLAGS_DEBUG >>> ${CMAKE_CXX_FLAGS_DEBUG}")
	message(STATUS "CMAKE_CXX_FLAGS_RELEASE >>> ${CMAKE_CXX_FLAGS_RELEASE}")
endmacro(print_info)

macro(set_output_paths)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin) 
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endmacro(set_output_paths)

macro(set_project_custom_defines)
	add_definitions(-DENGINE_DIRECTORY="${PROJECT_SOURCE_DIR}" -DSHADER_INCLUDE_DIRECTORY="${BGFX_DIR}/src")
endmacro()