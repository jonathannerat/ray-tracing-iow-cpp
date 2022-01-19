#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <string>

#include "HittableList.h"
#include "Triangle.h"

class TriangleMesh : public HittableList {
public:
  TriangleMesh(const std::string &objpath, shared_ptr<Material> m);
};

#endif // TRIANGLEMESH_H
