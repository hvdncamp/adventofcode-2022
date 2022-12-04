cmake_minimum_required(VERSION 3.16)
project(adventofcode_2022 C)

set(CMAKE_C_STANDARD 11)

include_directories(lib)

add_executable(day01_1 day01/part1.c lib/general_functions.h lib/general_functions.c)
add_executable(day01_2 day01/part2.c lib/general_functions.h lib/general_functions.c)
add_executable(day02_2 day02/part2.c lib/general_functions.h lib/general_functions.c)
add_executable(day02_1 day02/part1.c lib/general_functions.h lib/general_functions.c)
add_executable(day03_2 day03/part2.c lib/general_functions.h lib/general_functions.c)
add_executable(day03_1 day03/part1.c lib/general_functions.h lib/general_functions.c)


