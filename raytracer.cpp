#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"

static inline double clamp(double x, double a, double b){
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

Vec3 ray_color(const Ray& r, const Hittable& world, int depth){
    if (depth <= 0) return Vec3(0,0,0);

    HitRecord rec;
    if (world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return Vec3(0,0,0);
    }

    // background sky
    Vec3 unit_dir = normalize(r.direction);
    double t = 0.5 * (unit_dir.y + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

int main(){
    srand((unsigned)time(0));

    // image
    const int width = 400;
    const int height = 200;
    const int samples_per_pixel = 100;
    const int max_depth = 25;

    std::ofstream file("image.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

    Camera cam;

    // scene
    HittableList world;

    auto ground_mat = std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto center_mat = std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
    auto left_glass = std::make_shared<Dielectric>(1.5);
    auto right_mtl  = std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<Sphere>(Vec3(0,-100.5,-1), 100.0, ground_mat));
    world.add(std::make_shared<Sphere>(Vec3(0,    0, -1),   0.5, center_mat));
    world.add(std::make_shared<Sphere>(Vec3(-1,   0, -1),   0.5, left_glass));
    // hollow glass sphere (optional interior)
    world.add(std::make_shared<Sphere>(Vec3(-1,   0, -1),  -0.45, left_glass));
    world.add(std::make_shared<Sphere>(Vec3( 1,   0, -1),   0.5, right_mtl));

    // render
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            Vec3 pixel(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (width  - 1);
                double v = (j + random_double()) / (height - 1);
                Ray r = cam.get_ray(u, v);
                pixel += ray_color(r, world, max_depth);
            }
            // average and gamma-correct (gamma=2)
            pixel /= double(samples_per_pixel);
            pixel = Vec3(std::sqrt(pixel.x), std::sqrt(pixel.y), std::sqrt(pixel.z));

            int ir = static_cast<int>(256 * clamp(pixel.x, 0.0, 0.999));
            int ig = static_cast<int>(256 * clamp(pixel.y, 0.0, 0.999));
            int ib = static_cast<int>(256 * clamp(pixel.z, 0.0, 0.999));
            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    file.close();
    return 0;
}
