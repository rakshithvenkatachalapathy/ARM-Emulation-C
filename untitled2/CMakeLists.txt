cmake_minimum_required(VERSION 3.15)
project(untitled2 C)

set(CMAKE_C_STANDARD 99)

add_library(untitled2 library.c library.h cache.c armemu.c)