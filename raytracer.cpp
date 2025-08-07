#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"

inline double random_double(){
    return rand() / (RAND_MAX + 1.0);
}

Vec3 ray_color(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, 0.001, INFINITY, rec)) {
        Vec3 light_dir = normalize(Vec3(1, 1, 1));
        double brightness = std::max(0.0, dot(rec.normal, light_dir));
        Vec3 base_color(0.6, 0.3, 1.0); // violet-blue
        return brightness * rec.color;
    }

    // Sky gradient
    Vec3 unit_direction = normalize(r.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

int main() {
    srand(time(0));

    
    const int width = 400;
    const int height = 200;
    int samples_per_pixel = 100;

    std::ofstream file("image.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

    // Camera setup
    Camera cam;

    // Scene setup
    HittableList world;
    Sphere sphere1(Vec3(0, 0, -1), 0.5, Vec3(0.6, 0.3, 1.0));   // violet
    Sphere ground(Vec3(0, -100.5, -1), 100, Vec3(1, 01, 0.5)); // Ground as a giant sphere

    world.add(&sphere1);
    world.add(&ground);

    // Image generation loop
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            Vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (width - 1);
                double v = (j + random_double()) / (height - 1);
                Ray r = cam.get_ray(u, v);
                color += ray_color(r, world);
            }
            color /= double(samples_per_pixel);

            color = Vec3(sqrt(color.x), sqrt(color.y), sqrt(color.z));


            int ir = static_cast<int>(255.99 * color.x);
            int ig = static_cast<int>(255.99 * color.y);
            int ib = static_cast<int>(255.99 * color.z);
            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    file.close();
    return 0;
}
