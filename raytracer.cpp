#include <iostream>
#include <fstream>
#include "vec3.hpp"
#include "ray.hpp"

Vec3 ray_color(const Ray& r) {
    // Check ray-sphere intersection
    Vec3 center(0, 0, -1);
    double radius = 0.5;
    Vec3 oc = r.origin - center;

    double a = dot(r.direction, r.direction);
    double b = 2.0 * dot(oc, r.direction);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
       return Vec3(0.5, 0.0, 1.0);
    }

    // Background color
    Vec3 unit_direction = normalize(r.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}


int main() {
    const int width = 400;
    const int height = 200;

    std::ofstream file("image.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

    Vec3 origin(0, 0, 0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, 1);

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            double u = double(i) / (width - 1);
            double v = double(j) / (height - 1);

            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Vec3 color = ray_color(r);

            int ir = static_cast<int>(255.99 * color.x);
            int ig = static_cast<int>(255.99 * color.y);
            int ib = static_cast<int>(255.99 * color.z);

            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    file.close();
    return 0;
}
