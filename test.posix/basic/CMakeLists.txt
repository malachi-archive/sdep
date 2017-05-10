cmake_minimum_required(VERSION 3.7)
project(basic)

file(GLOB_RECURSE lib_sources ../../src/*.cpp)
include_directories(../../src)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ${lib_source})
add_executable(basic ${SOURCE_FILES})