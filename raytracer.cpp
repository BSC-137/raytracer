#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <typeinfo>
#include <cstdlib>

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "xz_rect.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "diffuse_light.hpp"
#include "onb.hpp"

// ----------------------- RENDER CONFIG -----------------------
static const bool motion_blur_enabled = false;
static const bool depth_of_field      = true;

static const double F_NUMBER = 2.0;
static const double SHUTTER  = 1.0/30;
static const int    ISO      = 400;

static const double EXPOSURE_COMP = 8.0;

// Direct-lighting quality: more samples => less noise
static const int LIGHT_SAMPLES_PER_HIT = 8;    // 4–16 is good
static const int BRDF_SAMPLES_PER_HIT  = 1;    // 1 is enough with MIS
// -------------------------------------------------------------

static inline double clamp01(double x){ if(x<0) return 0; if(x>1) return 1; return x; }

static inline double exposure_scale(double fnum, double shutter_s, int iso){
    double EV100 = std::log2((fnum*fnum)/shutter_s);
    return 0.18 * std::pow(2.0, -EV100) * (100.0/double(iso));
}

static inline Vec3 aces_tonemap(const Vec3& c){
    const double a=2.51,b=0.03,c2=2.43,d=0.59,e=0.14;
    auto tm=[&](double x){ double num=x*(a*x+b), den=x*(c2*x+d)+e; return clamp01(num/den); };
    return Vec3(tm(c.x), tm(c.y), tm(c.z));
}

// Helper: if this material is Lambertian, return its albedo; else (specular) return (-1)
static inline bool get_lambert_albedo(const std::shared_ptr<Material>& m, Vec3& out_albedo){
    auto* lam = dynamic_cast<Lambertian*>(m.get());
    if (!lam) return false;
    out_albedo = lam->albedo;
    return true;
}

// Compute solid-angle PDF for the rectangle light given a surface point and direction.
// Returns true if that direction hits the rect; fills pdf_omega, cos_l, dist.
static bool rect_pdf_omega_from_dir(const XZRect& rect, const Vec3& p, const Vec3& wi,
                                    double& pdf_omega, double& cos_l, double& dist)
{
    // Ray-plane intersection with plane y = k
    if (std::fabs(wi.y) < 1e-8) return false;
    double t = (rect.k - p.y) / wi.y;
    if (t <= 0.001) return false;

    Vec3 hit = p + wi * t;
    if (hit.x < rect.x0 || hit.x > rect.x1 || hit.z < rect.z0 || hit.z > rect.z1) return false;

    dist = t; // wi is unit
    cos_l = std::max(0.0, dot(rect.light_normal(), -wi));
    if (cos_l <= 0.0) return false;

    double A = rect.area();
    double dist2 = dist * dist;
    pdf_omega = dist2 / (cos_l * A);   // area -> solid angle
    return pdf_omega > 1e-12;
}

Vec3 ray_color(const Ray& r, const Hittable& world, const XZRect* area_light, int depth){
    if (depth <= 0) return Vec3(0,0,0);

    HitRecord rec;
    if (!world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        return Vec3(0,0,0); // black background (studio)
    }

    Vec3 emitted = rec.mat->emitted(rec);

    // If purely emissive (or specular delta handled in material), stop here
    Ray scattered;
    Vec3 attenuation;
    if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }

    // Recursive indirect (kept as before)
    Vec3 indirect = attenuation * ray_color(scattered, world, area_light, depth - 1);

    // -------- Direct lighting with MIS (Lambertian only) --------
    Vec3 direct(0,0,0);
    Vec3 albedo;
    if (area_light && get_lambert_albedo(rec.mat, albedo)) {
        // LIGHT SAMPLING: sample the rect a few times; weight with balance heuristic
        Vec3 L_light(0,0,0);
        for (int s=0; s<LIGHT_SAMPLES_PER_HIT; ++s) {
            Vec3 lp = area_light->sample_point();
            Vec3 toL = lp - rec.point;
            double dist2 = dot(toL, toL);
            if (dist2 <= 1e-12) continue;

            double dist = std::sqrt(dist2);
            Vec3 wi = toL / dist;

            double cos_i = std::max(0.0, dot(rec.normal, wi));
            double cos_l = std::max(0.0, dot(area_light->light_normal(), -wi));
            if (cos_i <= 0.0 || cos_l <= 0.0) continue;

            // visibility
            Ray shadow_ray(rec.point, wi);
            HitRecord shadow_hit;
            bool blocked = world.hit(shadow_ray, 0.001, dist - 0.001, shadow_hit);
            if (blocked) continue;

            double A = area_light->area();
            double pdf_light = dist2 / (cos_l * A);           // p_light(wi)
            double pdf_brdf  = cos_i / PI;                    // p_brdf(wi) for Lambertian
            double w = pdf_light / (pdf_light + pdf_brdf);    // balance heuristic

            Vec3 Le = area_light->mat->emitted(rec);
            Vec3 f  = (albedo / PI);                          // Lambertian BRDF
            L_light += w * Le * f * (cos_i / pdf_light);
        }
        L_light /= double(LIGHT_SAMPLES_PER_HIT);

        // BRDF SAMPLING: sample cosine direction; if it hits the light, add it with MIS
        Vec3 L_brdf(0,0,0);
        ONB onb; onb.build_from_w(rec.normal);
        for (int s=0; s<BRDF_SAMPLES_PER_HIT; ++s) {
            Vec3 local = random_cosine_direction();
            Vec3 wi = onb.local(local);     // world space
            double cos_i = std::max(0.0, dot(rec.normal, wi));
            if (cos_i <= 0.0) continue;

            // does this direction hit the rect?
            double pdf_light=0, cos_l=0, dist=0;
            if (!rect_pdf_omega_from_dir(*area_light, rec.point, wi, pdf_light, cos_l, dist))
                continue;

            // visibility
            Ray shadow_ray(rec.point, wi);
            HitRecord shadow_hit;
            bool blocked = world.hit(shadow_ray, 0.001, dist - 0.001, shadow_hit);
            if (blocked) continue;

            double pdf_brdf = cos_i / PI;                    // p_brdf(wi)
            double w = pdf_brdf / (pdf_brdf + pdf_light);    // balance heuristic

            Vec3 Le = area_light->mat->emitted(rec);
            Vec3 f  = (albedo / PI);
            L_brdf += w * Le * f * (cos_i / pdf_brdf);
        }
        if (BRDF_SAMPLES_PER_HIT > 0)
            L_brdf /= double(BRDF_SAMPLES_PER_HIT);

        direct = L_light + L_brdf;
    }

    return emitted + direct + indirect;
}

int main(){
    srand((unsigned)time(0));

    // Image
    const int width  = 640;
    const int height = 360;
    const int samples_per_pixel = 100;
    const int max_depth = 25;
    const double aspect = double(width) / double(height);

    std::ofstream file("image.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

    // Camera
    Vec3 lookfrom(3, 2, 2.5);
    Vec3 lookat(0, 0, -1);
    double focus_dist = (lookfrom - lookat).length();

    double aperture = depth_of_field ? 0.12 : 0.0;
    double time0 = 0.0, time1 = motion_blur_enabled ? 1.0 : 0.0;

    Camera cam(lookfrom, lookat, Vec3(0,1,0),
               /*vfov*/ 45.0, aspect,
               aperture, focus_dist,
               time0, time1);

    // Materials
    auto ground_mat = std::make_shared<Lambertian>(Vec3(0.75, 0.75, 0.75));
    auto blue_diff  = std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.7));
    auto glass      = std::make_shared<Dielectric>(1.5);
    auto steel      = std::make_shared<Metal>(Vec3(0.75, 0.75, 0.75), 0.05);

    // Light (area emitter)
    auto light_mat  = std::make_shared<DiffuseLight>(Vec3(1.0, 0.97, 0.92), /*exitance*/ 8000.0);

    // World
    HittableList world;
    world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100.0, ground_mat));
    world.add(std::make_shared<Sphere>(Vec3( 1.1, 0.0, -1.2), 0.5, steel));
    world.add(std::make_shared<Sphere>(Vec3(-1.2, 0.0, -1.0), 0.5, glass));
    world.add(std::make_shared<Sphere>(Vec3(-1.2, 0.0, -1.0), -0.45, glass));

    // Area light: y = 1.8; ensure XZRect::light_normal() returns Vec3(0,-1,0)
    auto rect_light = std::make_shared<XZRect>(-0.6, 0.6, -0.4, 0.4, /*y=*/1.8, light_mat);
    world.add(rect_light);

    // Exposure
    double exposure = exposure_scale(F_NUMBER, SHUTTER, ISO) * EXPOSURE_COMP;

    // Render
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            Vec3 pixel(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (width  - 1);
                double v = (j + random_double()) / (height - 1);
                Ray r = cam.get_ray(u, v);
                pixel += ray_color(r, world, rect_light.get(), max_depth);
            }

            pixel /= double(samples_per_pixel);       // HDR linear average
            pixel *= exposure;                        // exposure gain
            Vec3 mapped = aces_tonemap(pixel);        // tone map
            mapped = Vec3(std::sqrt(mapped.x), std::sqrt(mapped.y), std::sqrt(mapped.z)); // gamma

            int ir = int(256 * clamp01(mapped.x));
            int ig = int(256 * clamp01(mapped.y));
            int ib = int(256 * clamp01(mapped.z));
            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    file.close();
    return 0;
}
