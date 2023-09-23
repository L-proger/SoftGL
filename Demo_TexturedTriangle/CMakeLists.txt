cmake_minimum_required(VERSION 3.16)

project(Demo CXX C ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_executable(Demo_TexturedTriangle)

target_sources(Demo_TexturedTriangle
PRIVATE
    app.cpp
)