SRCDIR = src
SRC = $(shell find $(SRCDIR)/ -type f -name '*.cpp')
OBJ = $(patsubst %.cpp, %.o, $(SRC))

CFLAGS = -std=c++17 -Wall
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lm

all: iow

iow: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ)

src/rtweekend.h: src/Ray.h src/Vec3.h
src/main.cpp: src/rtweekend.h src/Camera.h src/hittable/Sphere.h src/materials/Material.h src/hittable/HittableList.h src/color.h
src/hittable/HittableList.cpp: src/hittable/HittableList.h
src/hittable/Sphere.cpp: src/hittable/Sphere.h

run: iow
	./iow | convert ppm:- out.png
	sxiv out.png

clean:
	-rm iow
	-rm $(OBJ)

.PHONY = all clean run
