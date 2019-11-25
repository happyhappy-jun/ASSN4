cmake_minimum_required(VERSION 3.15)
project(ASSN4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -O3 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
add_executable(ASSN4 20190766_assn4.c)