#include "Box.h"

using namespace std;

Box::Box(Point3 cb, Point3 cf, const shared_ptr<Material> &m) : mat_ptr(m) {
    cback = cb < cf ? cb : Point3(MIN(cb.x, cf.x), MIN(cb.y, cf.y), MIN(cb.z, cf.z));
    cfront = cb < cf ? cf : Point3(MAX(cb.x, cf.x), MAX(cb.y, cf.y), MAX(cb.z, cf.z));
    refp = (cback + cfront) / 2;

    Vec3 x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);

    faces = {
            Plane(cb, -x, m),
            Plane(cb, -y, m),
            Plane(cb, -z, m),
            Plane(cf, x, m),
            Plane(cf, y, m),
            Plane(cf, z, m),
    };
}

bool Box::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    HitRecord tmp_rec;
    double closest_so_far = t_max;
    bool hit_anything = false;

    for (auto &face: faces) {
        if (face.hit(r, t_min, closest_so_far, tmp_rec) && is_inside(tmp_rec.p)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}

bool Box::is_inside(const Point3 &p) const {
    bool inside = true;

    inside &= cback.x - EPS <= p.x && p.x <= cfront.x + EPS;
    inside &= cback.y - EPS <= p.y && p.y <= cfront.y + EPS;
    inside &= cback.z - EPS <= p.z && p.z <= cfront.z + EPS;

    return inside;
}

Box Box::operator+(const Box &o) const {
    Point3 cb = cback, cf = cfront;

    if (!is_inside(o.cback)) {
        cb.x = MIN(cback.x, o.cback.x);
        cb.y = MIN(cback.y, o.cback.y);
        cb.z = MIN(cback.z, o.cback.z);
    }

    if (!is_inside(o.cfront)) {
        cf.x = MAX(cfront.x, o.cfront.x);
        cf.y = MAX(cfront.y, o.cfront.y);
        cf.z = MAX(cfront.z, o.cfront.z);
    }

    return { cb, cf, mat_ptr };
}

shared_ptr<const Box> Box::bounding_box() const { return shared_from_this(); }

void Box::scale(const Vec3 &s) {
    Box((cback - refp) * s + refp, (cfront - refp) * s + refp, mat_ptr);
}

void Box::move(const Vec3 &o) {
    cback += o;
    cfront += o;
    refp += o;
}