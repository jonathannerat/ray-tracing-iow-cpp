#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"
#include "Vec3.h"

#include <iostream>

void write_color(std::ostream &out, Color pixel, int spp) {
	double r = pixel.x;
	double g = pixel.y;
	double b = pixel.z;
	double scale = 1.0 / spp;

	r = sqrt(r * scale);
	g = sqrt(g * scale);
	b = sqrt(b * scale);

	out << static_cast<int>(256 * clamp(r, 0, 1)) << ' '
	    << static_cast<int>(256 * clamp(g, 0, 1)) << ' '
	    << static_cast<int>(256 * clamp(b, 0, 1)) << '\n';
}

#endif // COLOR_H
