cmake_minimum_required(VERSION 3.24)
project(hello C)

set(CMAKE_C_STANDARD 99)

add_executable(hello main.c mongoose.c mongoose.h)
