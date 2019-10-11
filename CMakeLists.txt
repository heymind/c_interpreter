cmake_minimum_required(VERSION 3.14)
project(c_interpreter C)

set(CMAKE_C_STANDARD 11)

add_executable(c_interpreter main.c lex.c lex.h)