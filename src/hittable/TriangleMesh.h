#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <string>

#include "HittableList.h"
#include "Triangle.h"

class TriangleMesh : public HittableList<Triangle> {
public:
  TriangleMesh(const std::string &objpath, shared_ptr<Material> m);

  virtual void scale(const Vec3 &s) override;
};

#endif // TRIANGLEMESH_H
