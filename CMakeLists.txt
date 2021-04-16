cmake_minimum_required(VERSION 3.17)
project(C C)


set(CMAKE_C_STANDARD 11)

add_executable(C main.c libnova/ln_types.h libnova/julian_day.h libnova/utility.h julian_day.c)