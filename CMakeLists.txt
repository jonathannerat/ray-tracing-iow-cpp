cmake_minimum_required(VERSION 3.0)

project(RayTracingIOW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Descomentar para deshabilitar ImageMagick
#add_definitions(-DNO_IMAGEMAGICK)
add_executable(iow
	src/main.cpp src/Vec3.cpp src/Scene.cpp
	src/hittable/Sphere.cpp src/hittable/Plane.cpp src/hittable/Triangle.cpp src/hittable/Box.cpp src/hittable/TriangleMesh.cpp)

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(iow ${ImageMagick_LIBRARIES})
