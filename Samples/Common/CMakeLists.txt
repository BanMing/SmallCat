
file(GLOB_RECURSE commonsrc *.h *.cpp *.hpp *.c *.cc)

add_library (Common ${commonsrc})
