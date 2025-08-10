#pragma once
#include <memory>
#include "ray.hpp"
#include "aabb.hpp"

class Material; // forward declaration

struct HitRecord {
    Vec3 point;
    Vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<Material> mat;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal){
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(AABB& out_box) const = 0;
    virtual ~Hittable() = default;
};
