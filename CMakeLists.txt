cmake_minimum_required(VERSION 3.17)
project(rayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(rayTracing main.cpp vector.h ray.h)