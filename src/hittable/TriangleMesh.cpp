#include <fstream>
#include <iostream>

#include "Triangle.h"
#include "TriangleMesh.h"

using namespace std;

TriangleMesh::TriangleMesh(const string &objpath, shared_ptr<Material> m) {
  string line;
  vector<Point3> vertex;
  ifstream objfile(objpath);
  size_t parsed_lines = 0;

  while (getline(objfile, line)) {
    if (line.empty() && line[0] != 'v' && line[0] != 'f')
      continue;

    size_t pos = 1;
    switch (line[0]) {
    case 'v':
      while (line[pos] == ' ')
        pos++;

      if (pos != 1) {
        size_t nextpos = line.find(' ', pos) + 1;
        double x = stod(line.substr(pos, nextpos - pos - 1));

        pos = nextpos;
        nextpos = line.find(' ', pos) + 1;
        double y = stod(line.substr(pos, nextpos - pos - 1));

        double z = stod(line.substr(nextpos));

        vertex.push_back(Point3(x, y, z));
      }
      break;
    case 'f':
      while (line[pos] == ' ')
        pos++;

      if (pos != 1) {
        size_t nextpos = line.find(' ', pos) + 1;
        size_t i1 = stoi(line.substr(pos, nextpos - pos));

        pos = nextpos;
        nextpos = line.find(' ', pos) + 1;
        size_t i2 = stoi(line.substr(pos, nextpos - pos));

        size_t i3 = stoi(line.substr(nextpos));

        add(make_shared<Triangle>(vertex[i1 - 1], vertex[i2 - 1],
                                  vertex[i3 - 1], m));
      }
      break;
    }
  }
}

void TriangleMesh::scale(const Vec3 &s) {
    optional<Box> newbox;

    for (const auto &object : objects) {
        object->scale_inplace(s);

        if (!newbox.has_value())
            newbox.emplace(*object->bounding_box());
        else
            newbox.emplace(*newbox + *object->bounding_box());
    }

    // falla si no tiene valor, lo cual no deberia pasar
    box = make_shared<const Box>(*newbox);
}
