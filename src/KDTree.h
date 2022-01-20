#ifndef KDTREE_H
#define KDTREE_H

#include <optional>
#include <vector>

#include "hittable/Box.h"
#include "hittable/HittableList.h"

class KDTree : public Hittable {
public:
  size_t leaf_size;
  KDTree(const HittableList &list, size_t ls);

  size_t size() const;

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual std::shared_ptr<const Box> bounding_box() const override;

private:
  struct Node : public Hittable {
    std::unique_ptr<Node> left, right;
    std::optional<HittableList> objects;

    Node(const HittableList &list);

    void split(size_t leaf_size);
    size_t size() const;

    virtual bool hit(const Ray &r, double t_min, double t_max,
                     HitRecord &rec) const override;

    virtual std::shared_ptr<const Box> bounding_box() const override;

  private:
    std::shared_ptr<const Box> box;
  };

  Node root;
};

#endif // KDTREE_H
