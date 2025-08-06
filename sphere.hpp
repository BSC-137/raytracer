#pragma once
#include "hittable.hpp"

class Sphere : public Hittable {
public:
    Vec3 center;
    double radius;
    Vec3 color;

    Sphere(Vec3 c, double r, Vec3 col) : center(c), radius(r), color(col) {}

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        Vec3 oc = r.origin - center;
        double a = dot(r.direction, r.direction);
        double b = dot(oc, r.direction);
        double c = dot(oc, oc) - radius * radius;
        double discriminant = b * b - a * c;

        if (discriminant > 0) {
            double sqrt_d = std::sqrt(discriminant);
            double root = (-b - sqrt_d) / a;
            if (root < t_max && root > t_min) {
                rec.t = root;
                rec.point = r.at(rec.t);
                rec.normal = normalize(rec.point - center);
                rec.color = color; // Set the color here
                return true;
            }

            root = (-b + sqrt_d) / a;
            if (root < t_max && root > t_min) {
                rec.t = root;
                rec.point = r.at(rec.t);
                rec.normal = normalize(rec.point - center);
                rec.color = color; // Set the color here
                return true;
            }
        }

        return false;
    }
};
