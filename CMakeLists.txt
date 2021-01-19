cmake_minimum_required(VERSION 3.17)
project(rayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -pthread -fopenmp")
#SET(CMAKE_CXX_FLAGS "/O2 /openmp")
add_executable(rayTracing main.cpp Vector.h Ray.h Collide.h Sphere.h CollisibleList.h Camera.h Material.h Lambertian.h Random.h Metal.h Glass.h MovingSphere.h AABB.h BVH.h Texture.h stb_image.h DiffuseLight.h)