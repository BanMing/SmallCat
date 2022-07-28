file(GLOB_RECURSE libsrc *.h *.cpp *.hpp *.c *.cc)

add_library (Src ${libsrc})

target_link_libraries(Src PUBLIC core)
target_link_libraries(Src PUBLIC mml-window)

target_include_directories (Src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)