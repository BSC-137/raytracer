#pragma once
#include "ray.hpp"
#include "hittable.hpp"

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered) const = 0;
    virtual ~Material() = default;
};
