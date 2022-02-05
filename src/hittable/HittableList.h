#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <type_traits>
#include <vector>
#include <optional>

#include "Box.h"
#include "hittable.h"

using namespace std;

template<class T = Hittable>
struct HittableList : public Hittable {
    static_assert(is_base_of<Hittable, T>::value,
                  "T must inherit from Hittable");
    vector<shared_ptr<T>> objects;

    HittableList() {};

    HittableList(const HittableList &o) : box(o.box), objects(o.objects) {};

    HittableList(shared_ptr<T> object) { add(object); };

    HittableList<T> operator=(const HittableList<T> &o) {
        box = o.box;
        objects = o.objects;
        return *this;
    }

    void clear() {
        objects.clear();
        refpacc = Point3();
    }

    size_t size() const { return objects.size(); };

    void add(shared_ptr<T> object) {
        shared_ptr<const Box> objbox = object->bounding_box();

        objects.push_back(object);

        refpacc += object->refp;
        refp = refpacc / objects.size();

        if (!box)
            box = objbox;
        else if (objbox)
            box = make_shared<Box>(*box + *objbox);
    }

    bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override {
        HitRecord tmp_rec;
        bool hit_anything = false;
        double closest_so_far = t_max;

        for (auto object: objects) {
            if (object->hit(r, t_min, closest_so_far, tmp_rec)) {
                hit_anything = true;
                closest_so_far = tmp_rec.t;
                rec = tmp_rec;
            }
        }

        return hit_anything;
    }

    shared_ptr<const Box> bounding_box() const override { return box; }

    void scale(const Vec3 &s) override {
        optional<Box> newbox;

        for (const shared_ptr<Hittable> &object: objects) {
            object->scale(s);

            if (!newbox.has_value())
                newbox.emplace(*object->bounding_box());
            else
                newbox.emplace(*newbox + *object->bounding_box());
        }

        // falla si no tiene valor, lo cual no deberia pasar
        box = make_shared<const Box>(newbox.value());
    }

    void move(const Vec3 &o) override {
        for (auto &object: objects) object->move(o);

        auto newbox = make_shared<Box>(*box);
        newbox->move(o);
        box = newbox;
    }

protected:
    shared_ptr<const Box> box;

private:
    Point3 refpacc;
};

#endif // HITTABLELIST_H
