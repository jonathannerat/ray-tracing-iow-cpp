cmake_minimum_required(VERSION 3.0)

project(RayTracingIOW)

add_executable(iow
	src/main.cpp
	src/Vec3.cpp
	src/hittable/HittableList.cpp src/hittable/Sphere.cpp src/hittable/Plane.cpp src/hittable/Triangle.cpp src/hittable/Box.cpp src/hittable/TriangleMesh.cpp)
