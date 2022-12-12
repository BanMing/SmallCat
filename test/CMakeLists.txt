file(GLOB_RECURSE TestSrc test/*.h test/*.cpp)

add_executable( test  ${TestSrc} ${MathSrc} ${AniamtionSrc} ${GltfSrc})
target_link_libraries(test PUBLIC bx bimg bgfx example-common)