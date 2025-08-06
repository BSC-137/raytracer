#pragma once
#include "ray.hpp"

struct HitRecord {
    Vec3 point;
    Vec3 normal;
    double t;
    Vec3 color;
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};
