cmake_minimum_required(VERSION 3.30)
project(xModem)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(xModem
        instructions.hpp
        main.cpp
        Reader.cpp
        Reader.hpp)
