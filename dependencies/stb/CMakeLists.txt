cmake_minimum_required(VERSION 3.0)

project(stb)

add_library(stb STATIC
        src/stb_vorbis.c
        src/stb_lib.cpp
        )
target_include_directories(stb
        PUBLIC include/
        )
