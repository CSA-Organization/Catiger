cmake_minimum_required(VERSION 4.3)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(Catiger LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 26)

add_executable(GuideCompiler
        GuideCompiler/Main.cpp
        GuideCompiler/AbstractStream.hpp
        GuideCompiler/AbstractStream.cpp
)