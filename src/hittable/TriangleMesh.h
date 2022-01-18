#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <string>

#include "Triangle.h"
#include "HittableList.h"

class TriangleMesh : public HittableList {
	public:
	TriangleMesh(const std::string &objpath, shared_ptr<Material> m);
};

#endif // TRIANGLEMESH_H
