#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include <optional>
#include <vector>

#include "hittable/Box.h"
#include "hittable/HittableList.h"

using std::make_unique;
using std::unique_ptr;

template<class T = Hittable>
class KDTree : public Hittable {
    static_assert(std::is_base_of<Hittable, T>::value,
                  "T must inherit from Hittable");

public:
    KDTree(const HittableList<T> &list, size_t ls) : root(list) {
        root.split(ls);
        refp = list.refp;
    }

    size_t size() const { return root.size(); }

    bool hit(const Ray &r, double t_min, double t_max,
             HitRecord &rec) const override {
        return root.hit(r, t_min, t_max, rec);
    }

    shared_ptr<const Box> bounding_box() const override {
        return root.bounding_box();
    }

    void scale(const Vec3 &s) override { root.scale(s); };

    void move(const Vec3 &o) override { root.move(o); };

private:
    struct Node : public Hittable {
        std::unique_ptr<Node> left, right;
        std::optional<HittableList<T>> objects;

        explicit Node(const HittableList<T> &list)
                : objects(list), box(list.bounding_box()) {
            refp = list.refp;
        }

        void split(size_t leaf_size) {
            if (!objects.has_value())
                return;

            // calcular eje mas largo
            double maxdiff = 0;
            char axis;
            auto bbox = bounding_box();

            if (bbox->cfront.x - bbox->cback.x > maxdiff) {
                maxdiff = bbox->cfront.x - bbox->cback.x;
                axis = 'x';
            }

            if (bbox->cfront.y - bbox->cback.y > maxdiff) {
                maxdiff = bbox->cfront.y - bbox->cback.y;
                axis = 'y';
            }

            if (bbox->cfront.z - bbox->cback.z > maxdiff) {
                axis = 'z';
            }

            // ordenar los objetos en ese eje
            auto objsort = objects.value().objects;

            sort(objsort.begin(), objsort.end(),
                 [axis](const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b) {
                     if (axis == 'x')
                         return a->refp.x < b->refp.x;
                     else if (axis == 'y')
                         return a->refp.y < b->refp.y;
                     else
                         return a->refp.z < b->refp.z;
                 });

            // dividirlos en los nodos left y right
            size_t middle = objsort.size() / 2;

            HittableList<T> hlleft, hlright;

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

        size_t size() const {
            if (objects.has_value())
                return objects.value().size();

            return left->size() + right->size();
        }

        bool hit(const Ray &r, double t_min, double t_max,
                 HitRecord &rec) const override {
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

        shared_ptr<const Box> bounding_box() const override { return box; }

        void scale(const Vec3 &s) override {
            if (left && right) {
                left->scale(s);
                right->scale(s);
                box = make_shared<const Box>(*left->bounding_box() + *right->bounding_box());
            } else if (objects.has_value()) {
                objects.value().scale(s);
                box = objects.value().bounding_box();
            }
        };

        void move(const Vec3 &o) override {
            if (left && right) {
                left->move(o);
                right->move(o);
            } else if (objects.has_value()) {
                objects.value().move(o);
            }

            refp += o;

            auto newbox = make_shared<Box>(*box);
            newbox->move(o);
            box = newbox;
        };

    private:
        std::shared_ptr<const Box> box;
    };

    Node root;
};

#endif // KDTREE_H
