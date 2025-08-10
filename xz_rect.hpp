#pragma once
#include <memory>
#include "hittable.hpp"

class XZRect : public Hittable {
public:
    double x0, x1, z0, z1, k; // lies on plane y = k
    std::shared_ptr<Material> mat;

    XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
           std::shared_ptr<Material> m)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat(std::move(m)) {}

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        // Intersect with plane y = k -> r.origin.y + t*r.direction.y = k
        double denom = r.direction.y;
        if (std::fabs(denom) < 1e-8) return false;
        double t = (k - r.origin.y) / denom;
        if (t < t_min || t > t_max) return false;

        double x = r.origin.x + t * r.direction.x;
        double z = r.origin.z + t * r.direction.z;
        if (x < x0 || x > x1 || z < z0 || z > z1) return false;

        rec.t = t;
        rec.point = r.at(t);
        // Rectangle normal: up if ray comes from above, down if from below
        Vec3 outward_normal = Vec3(0, 1, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        return true;
    }

    double area() const { return (x1 - x0) * (z1 - z0); }

    // Uniformly sample a point on the rect (area measure)
    Vec3 sample_point() const {
        double rx = random_double(x0, x1);
        double rz = random_double(z0, z1);
        return Vec3(rx, k, rz);
    }

    // Normal of the light (downward in object space)
    Vec3 light_normal() const { return Vec3(0, -1, 0); } // emit downward

};
