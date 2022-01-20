cmake_minimum_required(VERSION 3.0)

project(RayTracingIOW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(iow
	src/main.cpp
	src/Vec3.cpp
	src/KDTree.cpp
	src/hittable/HittableList.cpp src/hittable/Sphere.cpp src/hittable/Plane.cpp src/hittable/Triangle.cpp src/hittable/Box.cpp src/hittable/TriangleMesh.cpp)
