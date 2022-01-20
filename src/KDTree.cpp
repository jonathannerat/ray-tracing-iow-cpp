#include <algorithm>
#include <iostream>

#include "KDTree.h"

using namespace std;

KDTree::KDTree(const HittableList &list, size_t ls)
    : root(list), leaf_size(ls) {
  root.split(ls);
  refp = list.refp;
}

bool KDTree::hit(const Ray &r, double t_min, double t_max,
                 HitRecord &rec) const {
  return root.hit(r, t_min, t_max, rec);
}

shared_ptr<const Box> KDTree::bounding_box() const {
  return root.bounding_box();
}

size_t KDTree::size() const { return root.size(); }

KDTree::Node::Node(const HittableList &list)
    : objects(list), box(list.bounding_box()) {}

void KDTree::Node::split(size_t leaf_size) {
  if (!objects.has_value())
    return;

  // calcular eje mas largo
  double maxdiff = 0;
  char axis;
  auto box = bounding_box();

  if (box->cfront.x - box->cback.x > maxdiff) {
    maxdiff = box->cfront.x - box->cback.x;
    axis = 'x';
  }

  if (box->cfront.y - box->cback.y > maxdiff) {
    maxdiff = box->cfront.y - box->cback.y;
    axis = 'y';
  }

  if (box->cfront.z - box->cback.z > maxdiff) {
    maxdiff = box->cfront.z - box->cback.z;
    axis = 'z';
  }

  // ordenar los objetos en ese eje
  auto objsort = objects.value().objects;

  sort(objsort.begin(), objsort.end(),
       [axis](shared_ptr<Hittable> a, shared_ptr<Hittable> b) {
         if (axis == 'x')
           return a->refp.x < b->refp.x;
         else if (axis == 'y')
           return a->refp.y < b->refp.y;
         else
           return a->refp.z < b->refp.z;
       });

  // dividirlos en los nodos left y right
  size_t middle = objsort.size() / 2;

  HittableList hlleft, hlright;

  for (size_t i = 0; i < middle; i++)
    hlleft.add(objsort[i]);

  for (size_t i = middle; i < objsort.size(); i++)
    hlright.add(objsort[i]);

  left = make_unique<KDTree::Node>(hlleft);
  right = make_unique<KDTree::Node>(hlright);

  if (objects.value().size() > leaf_size)
    objects.reset();

  if (hlleft.size() > leaf_size)
    left->split(leaf_size);
  if (hlright.size() > leaf_size)
    right->split(leaf_size);
}

bool KDTree::Node::hit(const Ray &r, double t_min, double t_max,
                       HitRecord &rec) const {
  auto bbox = bounding_box();
  HitRecord tmp_rec;

  if (!bbox || !bbox->hit(r, t_min, t_max, tmp_rec))
    return false;

  if (objects.has_value()) {
    return objects.value().hit(r, t_min, t_max, rec);
  } else {
    HitRecord lrec, rrec;
    bool lhit, rhit;

    if (left)
      lhit = left->hit(r, t_min, t_max, lrec);

    if (right)
      rhit = right->hit(r, t_min, t_max, rrec);

    if (lhit && rhit)
      rec = lrec.t < rrec.t ? lrec : rrec;
    else if (lhit || rhit)
      rec = lhit ? lrec : rrec;

    return lhit || rhit;
  }
}

shared_ptr<const Box> KDTree::Node::bounding_box() const { return box; }

size_t KDTree::Node::size() const {
  if (objects.has_value())
    return objects.value().size();

  return left->size() + right->size();
}
