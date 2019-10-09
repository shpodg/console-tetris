cmake_minimum_required(VERSION 3.0)
project(console-tetris)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
add_executable(console-tetris ConsoleCanvas.cpp main.cpp)