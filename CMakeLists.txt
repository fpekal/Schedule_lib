cmake_minimum_required(VERSION 3.0)

project(Schedule_lib)

add_library(Schedule_lib STATIC src/schedule_lib.cpp)
target_include_directories(Schedule_lib PUBLIC include)
