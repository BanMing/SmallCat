file(GLOB_RECURSE TestSrc test/*.h test/*.cpp)
add_executable( test  ${TestSrc} ${MathSrc} )