#pragma once
#include "ray.hpp"

class Camera {
public:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera() {
        origin = Vec3(0, 0, 0);
        horizontal = Vec3(4.0, 0.0, 0.0);
        vertical = Vec3(0.0, 2.0, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, 1);
    }

    Ray get_ray(double u, double v) const {
        return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
};
