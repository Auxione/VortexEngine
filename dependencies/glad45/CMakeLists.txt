cmake_minimum_required(VERSION 3.0)

project(glad33)

add_library(glad33
        src/glad.c
        )

target_include_directories(glad33
        PUBLIC include/
        )