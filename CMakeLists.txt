cmake_minimum_required(VERSION 4.3)
project(Catiger)

set(CMAKE_CXX_STANDARD 26)

add_executable(Compiler Compiler/Main.cpp
        Compiler/OStream.hpp
        Compiler/OStream.cpp
)