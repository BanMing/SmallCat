
cmake_minimum_required(VERSION 3.1)
project(Hello)

file(GLOB_RECURSE HELLO *.h *.cpp *.hpp *.c *.cc)

add_executable(Hello ${HELLO})